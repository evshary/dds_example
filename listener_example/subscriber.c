#include "dds/dds.h"
#include "ListenerData.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* An array of one message (aka sample in dds terms) will be used. */
#define MAX_SAMPLES 1

void my_subscription_matched(dds_entity_t reader, const dds_subscription_matched_status_t status, void* arg)
{
    printf ("[subscription-matched: total=(%"PRIu32" change %"PRId32") current=(%"PRIu32" change %"PRId32") handle=%"PRIu64"]\n",
            status.total_count, status.total_count_change,
            status.current_count, status.current_count_change,
            status.last_publication_handle);
}

int main (int argc, char ** argv)
{
    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t reader;
    ListenerData_Msg *msg;
    void *samples[MAX_SAMPLES];
    dds_sample_info_t infos[MAX_SAMPLES];
    dds_return_t rc;
    dds_qos_t *qos;
    dds_listener_t *rd_listener;
    (void)argc;
    (void)argv;
  
    /* Create a Participant. */
    participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
    if (participant < 0)
        DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));
  
    /* Create a Topic. */
    topic = dds_create_topic(participant, &ListenerData_Msg_desc, "ListenerData_Msg", NULL, NULL);
    if (topic < 0)
        DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));
  
    /* Setup QoS */
    qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
    /* Add listener */
    rd_listener = dds_create_listener(NULL);
    dds_lset_subscription_matched(rd_listener, my_subscription_matched);
    reader = dds_create_reader(participant, topic, qos, rd_listener);
    if (reader < 0)
        DDS_FATAL("dds_create_reader: %s\n", dds_strretcode(-reader));
    dds_delete_listener(rd_listener);
    dds_delete_qos(qos);
  
    printf("\n=== [Subscriber] Waiting for a sample ...\n");
    fflush(stdout);
  
    /* Initialize sample buffer, by pointing the void pointer within
     * the buffer array to a valid sample memory location. */
    samples[0] = ListenerData_Msg__alloc();
  
    /* Poll until data has been read. */
    while(true) {
        /* Do the actual read.
         * The return value contains the number of read samples. */
        rc = dds_take(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
        if (rc < 0)
            DDS_FATAL("dds_read: %s\n", dds_strretcode(-rc));
  
        /* Check if we read some data and it is valid. */
        if ((rc > 0) && (infos[0].valid_data)) {
            /* Print Message. */
            msg = (ListenerData_Msg*) samples[0];
            printf("=== [Subscriber] Received : ");
            printf("Message (%"PRId32", %s)\n", msg->userID, msg->message);
            fflush(stdout);
        }
        dds_sleepfor(DDS_MSECS(20));
    }
  
    /* Free the data location. */
    ListenerData_Msg_free(samples[0], DDS_FREE_ALL);
  
    /* Deleting the participant will delete all its children recursively as well. */
    rc = dds_delete(participant);
    if (rc != DDS_RETCODE_OK)
        DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));
  
    return EXIT_SUCCESS;
}

