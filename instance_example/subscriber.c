#include "dds/dds.h"
#include "InstanceData.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* An array of one message (aka sample in dds terms) will be used. */
#define MAX_SAMPLES 1

int main (int argc, char ** argv)
{
    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t reader;
    InstanceData_Msg *msg;
    void *samples[MAX_SAMPLES];
    dds_sample_info_t infos[MAX_SAMPLES];
    dds_return_t rc;
    dds_qos_t *qos;
    (void)argc;
    (void)argv;
  
    /* Create a Participant. */
    participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
    if (participant < 0)
        DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));
  
    /* Create a Topic. */
    topic = dds_create_topic(participant, &InstanceData_Msg_desc, "InstanceData_Msg", NULL, NULL);
    if (topic < 0)
        DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));
  
    /* Create a reliable Reader. */
    qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
    dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, DDS_LENGTH_UNLIMITED);
    reader = dds_create_reader(participant, topic, qos, NULL);
    if (reader < 0)
        DDS_FATAL("dds_create_reader: %s\n", dds_strretcode(-reader));
    dds_delete_qos(qos);
  
    // Wait for publisher ready
    dds_sleepfor(DDS_SECS (2));
  
    printf("\n=== [Subscriber] Waiting for a sample ...\n");
    fflush(stdout);
  
    samples[0] = InstanceData_Msg__alloc ();
  
    /* Register the instance to reader (key: id=0) */
    InstanceData_Msg instance_msg;
    instance_msg.userID = 0;
    dds_instance_handle_t hdl;
    hdl = dds_lookup_instance(reader, &instance_msg);
    while (1) {
        rc = dds_take_instance(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES, hdl);
        // There is no publisher register the instance now.
        if (rc == DDS_RETCODE_PRECONDITION_NOT_MET)
            continue;
        // Other error.
        if (rc < 0)
            DDS_FATAL("dds_read: %s\n", dds_strretcode(-rc));
  
        /* Check if we read some data and it is valid. */
        if ((rc > 0) && (infos[0].valid_data))
        {
            /* Print Message. */
            msg = (InstanceData_Msg*) samples[0];
            printf("=== [Subscriber] Received : ");
            printf("Message (%"PRId32", %d, %s)\n", msg->userID, msg->counter, msg->message);
            fflush(stdout);
        }
    }
  
    /* Free the data location. */
    InstanceData_Msg_free(samples[0], DDS_FREE_ALL);
  
    /* Deleting the participant will delete all its children recursively as well. */
    rc = dds_delete(participant);
    if (rc != DDS_RETCODE_OK)
        DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));
  
    return EXIT_SUCCESS;
}

