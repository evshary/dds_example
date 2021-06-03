#include "dds/dds.h"
#include "StatusData.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
    dds_entity_t participant;
    dds_entity_t topic;
    dds_entity_t writer;
    dds_return_t rc;
    StatusData_Msg msg;
    dds_qos_t *qos;
    uint32_t status = 0;
    long id_num;
    (void)argc;
    (void)argv;

    if (argc > 1)
        id_num = atoi(argv[1]);
    else
        id_num = 1;

    /* Create a Participant. */
    participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
    if (participant < 0)
        DDS_FATAL("dds_create_participant: %s\n", dds_strretcode(-participant));

    /* Create a Topic. */
    topic = dds_create_topic(participant, &StatusData_Msg_desc, "StatusData_Msg", NULL, NULL);
    if (topic < 0)
        DDS_FATAL("dds_create_topic: %s\n", dds_strretcode(-topic));

    /* Create a Writer. */
    qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS (10));
    dds_qset_durability(qos, DDS_DURABILITY_TRANSIENT_LOCAL);
    dds_qset_liveliness(qos, DDS_LIVELINESS_AUTOMATIC, DDS_SECS (5));
    writer = dds_create_writer(participant, topic, qos, NULL);
    if (writer < 0)
        DDS_FATAL("dds_create_writer: %s\n", dds_strretcode(-writer));
    dds_delete_qos(qos);

    printf("Run status publisher...\n");
    fflush(stdout);

    /* Create a message to write. */
    msg.userID = id_num;
    msg.message = "Hello!";

    printf("Status publisher: (%"PRId32", %s)\n", msg.userID, msg.message);
    fflush(stdout);

    rc = dds_write(writer, &msg);
    if (rc != DDS_RETCODE_OK)
        DDS_FATAL("dds_write: %s\n", dds_strretcode(-rc));

    while (1) {
      dds_sleepfor(DDS_MSECS (20));
    }

    /* Deleting the participant will delete all its children recursively as well. */
    rc = dds_delete(participant);
    if (rc != DDS_RETCODE_OK)
        DDS_FATAL("dds_delete: %s\n", dds_strretcode(-rc));

    return EXIT_SUCCESS;
}

