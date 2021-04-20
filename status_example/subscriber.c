#include "dds/dds.h"
#include "StatusData.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* An array of one message (aka sample in dds terms) will be used. */
#define MAX_SAMPLES 1
#define MAX_DEV_NUM 10

typedef struct {
    long deviceID;
    dds_instance_handle_t publication_handle;
} device_list;

device_list dev_list[MAX_DEV_NUM];

static void callback_liveliness_changed(dds_entity_t rd, const dds_liveliness_changed_status_t status, void* arg) {
    printf ("[liveliness-changed: alive=(%"PRIu32" change %"PRId32") not_alive=(%"PRIu32" change %"PRId32") handle=%"PRIu64"]\n",
            status.alive_count, status.alive_count_change,
            status.not_alive_count, status.not_alive_count_change,
            status.last_publication_handle);

    for (int i = 0; i < MAX_DEV_NUM; i++) {
      if (dev_list[i].publication_handle == status.last_publication_handle) {
        printf("Device ID %ld is lost\n", dev_list[i].deviceID);
        dev_list[i].deviceID = -1;
        dev_list[i].publication_handle = 0;
        break;
      }
    }
}

int main (int argc, char ** argv)
{
  dds_listener_t *listener;
  dds_entity_t participant;
  dds_entity_t topic;
  dds_entity_t reader;
  StatusData_Msg *msg;
  void *samples[MAX_SAMPLES];
  dds_sample_info_t infos[MAX_SAMPLES];
  dds_return_t rc;
  dds_qos_t *qos;
  (void)argc;
  (void)argv;

  // init device list
  for (int i = 0; i < MAX_DEV_NUM; i++) {
    dev_list[i].deviceID = -1;
    dev_list[i].publication_handle = 0;
  }

  /* Create a Participant. */
  participant = dds_create_participant (DDS_DOMAIN_DEFAULT, NULL, NULL);
  if (participant < 0)
    DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

  /* Create a Topic. */
  topic = dds_create_topic (
    participant, &StatusData_Msg_desc, "StatusData_Msg", NULL, NULL);
  if (topic < 0)
    DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

  /* Create a reliable Reader. */
  listener = dds_create_listener (NULL);
  dds_lset_liveliness_changed(listener, callback_liveliness_changed);
  qos = dds_create_qos ();
  dds_qset_reliability (qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
  dds_qset_durability (qos, DDS_DURABILITY_TRANSIENT_LOCAL);
  dds_qset_liveliness (qos, DDS_LIVELINESS_AUTOMATIC, DDS_SECS (5));
  reader = dds_create_reader (participant, topic, qos, listener);
  if (reader < 0)
    DDS_FATAL("dds_create_reader: %s\n", dds_strretcode(-reader));
  dds_delete_qos(qos);
  dds_delete_listener (listener);

  printf ("Waiting for status publishers ...\n");
  fflush (stdout);

  /* Initialize sample buffer, by pointing the void pointer within
   * the buffer array to a valid sample memory location. */
  samples[0] = StatusData_Msg__alloc ();

  /* Poll until data has been read. */
  while (true)
  {
    /* Do the actual read.
     * The return value contains the number of read samples. */
    rc = dds_take (reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
    if (rc < 0)
      DDS_FATAL("dds_read: %s\n", dds_strretcode(-rc));

    /* Check if we read some data and it is valid. */
    if ((rc > 0) && (infos[0].valid_data))
    {
      /* Print Message. */
      msg = (StatusData_Msg*) samples[0];
      printf ("Receive message from status publisher (%"PRId32", %s)\n", msg->userID, msg->message);
      int first_empty = -1;
      int found = 0;
      for(int i = 0; i < MAX_DEV_NUM; i++) {
        if (dev_list[i].deviceID == msg->userID) {
          found = 1;
          break;
        } else if (dev_list[i].deviceID == -1 && first_empty == -1) {
          first_empty = i;
        }
      }
      if (!found) {
        dev_list[first_empty].deviceID = msg->userID;
        dev_list[first_empty].publication_handle = infos[0].publication_handle;
      }
      fflush (stdout);
    }
    else
    {
      /* Polling sleep. */
      dds_sleepfor (DDS_MSECS (20));
    }
  }

  /* Free the data location. */
  StatusData_Msg_free (samples[0], DDS_FREE_ALL);

  /* Deleting the participant will delete all its children recursively as well. */
  rc = dds_delete (participant);
  if (rc != DDS_RETCODE_OK)
    DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));

  return EXIT_SUCCESS;
}

