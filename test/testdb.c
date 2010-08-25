
#include <stdio.h>
#include <string.h>

#include <lo/lo_lowlevel.h>

#include "../src/types_internal.h"
#include "../src/mapper_internal.h"

void printsignal(mapper_db_signal sig)
{
    printf("  name=%s%s, type=%c, length=%d",
           sig->device_name, sig->name, sig->type, sig->length);
    if (sig->unit)
        printf(", unit=%s", sig->unit);
    if (sig->minimum) {
        if (sig->type == 'i')
            printf(", minimum=%d", sig->minimum->i32);
        else if (sig->type == 'f')
            printf(", minimum=%g", sig->minimum->f);
    }
    if (sig->maximum) {
        if (sig->type == 'i')
            printf(", maximum=%d", sig->maximum->i32);
        else if (sig->type == 'f')
            printf(", maximum=%g", sig->maximum->f);
    }
    printf("\n");
}

int main()
{
    lo_arg *args[20];
    mapper_message_t msg;
    int port=1234;
    int zero=0, one=1;

    /* Test the database functions */

    args[0] = (lo_arg*)"@port";
    args[1] = (lo_arg*)&port;
    args[2] = (lo_arg*)"@IP";
    args[3] = (lo_arg*)"localhost";

    if (mapper_msg_parse_params(&msg, "/registered", "siss", 4, args))
    {
        printf("1: Error, parsing failed.\n");
        return 1;
    }

    mapper_db_add_or_update_device_params("/testdb.1", &msg);
    mapper_db_add_or_update_device_params("/testdb__.2", &msg);
    mapper_db_add_or_update_device_params("/testdb.3", &msg);
    mapper_db_add_or_update_device_params("/testdb__.4", &msg);

    args[0] = (lo_arg*)"@type";
    args[1] = (lo_arg*)"f";
    args[2] = (lo_arg*)"@IP";
    args[3] = (lo_arg*)"localhost";

    if (mapper_msg_parse_params(&msg, "/testdb.1/namespace/input",
                                "sc", 2, args))
    {
        printf("2: Error, parsing failed.\n");
        return 1;
    }

    mapper_db_add_or_update_signal_params("/in1", "/testdb.1", 0, &msg);
    mapper_db_add_or_update_signal_params("/in2", "/testdb.1", 0, &msg);
    mapper_db_add_or_update_signal_params("/in2", "/testdb.1", 0, &msg);

    mapper_db_add_or_update_signal_params("/out1", "/testdb.1", 1, &msg);
    mapper_db_add_or_update_signal_params("/out2", "/testdb.1", 1, &msg);
    mapper_db_add_or_update_signal_params("/out1", "/testdb.2", 1, &msg);

    args[0] = (lo_arg*)"@scaling";
    args[1] = (lo_arg*)"bypass";
    args[2] = (lo_arg*)"@clipMin";
    args[3] = (lo_arg*)"none";

    if (mapper_msg_parse_params(&msg, "/connected",
                                "ssss", 4, args))
    {
        printf("4: Error, parsing failed.\n");
        return 1;
    }

    mapper_db_add_or_update_mapping_params("/testdb.1/out2",
                                           "/testdb__.2/in1", &msg);
    mapper_db_add_or_update_mapping_params("/testdb__.2/out1",
                                           "/testdb.1/in1", &msg);

    args[0] = (lo_arg*)"@scaling";
    args[1] = (lo_arg*)"expression";
    args[2] = (lo_arg*)"@expression";
    args[3] = (lo_arg*)"(x-10)*80";
    args[4] = (lo_arg*)"@clipMin";
    args[5] = (lo_arg*)"none";
    args[6] = (lo_arg*)"@range";
    args[7] = (lo_arg*)&zero;
    args[8] = (lo_arg*)&one;
    args[9] = (lo_arg*)&zero;
    args[10] = (lo_arg*)&one;

    if (mapper_msg_parse_params(&msg, "/connected",
                                "sssssssiiii", 11, args))
    {
        printf("5: Error, parsing failed.\n");
        return 1;
    }

    mapper_db_add_or_update_mapping_params("/testdb.1/out1",
                                           "/testdb__.2/in2", &msg);
    mapper_db_add_or_update_mapping_params("/testdb__.2/out2",
                                           "/testdb.1/in2", &msg);

    if (mapper_msg_parse_params(&msg, "/linked",
                                "", 0, args))
    {
        printf("6: Error, parsing failed (on no args!)\n");
        return 1;
    }

    mapper_db_add_or_update_link_params("/testdb.1", "/testdb__.2", &msg);
    mapper_db_add_or_update_link_params("/testdb__.2", "/testdb.3", &msg);
    mapper_db_add_or_update_link_params("/testdb__.2", "/testdb.3", &msg);
    mapper_db_add_or_update_link_params("/testdb.3", "/testdb.1", &msg);
    mapper_db_add_or_update_link_params("/testdb__.2", "/testdb__.4", &msg);

    /*********/

    trace("Dump:\n");
    mapper_db_dump();

    /*********/

    printf("\n--- Devices ---\n");

    printf("\nWalk the whole database:\n");
    mapper_db_device *pdev = mapper_db_get_all_devices();
    int count=0;
    if (!pdev) {
        printf("mapper_db_get_all_devices() returned 0.\n");
        return 1;
    }
    if (!*pdev) {
        printf("mapper_db_get_all_devices() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (pdev) {
        count ++;
        printf("  name=%s, host=%s, port=%d, canAlias=%d\n",
               (*pdev)->name, (*pdev)->host,
               (*pdev)->port, (*pdev)->canAlias);
        pdev = mapper_db_device_next(pdev);
    }

    if (count != 4) {
        printf("Expected 4 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind /testdb.3:\n");

    mapper_db_device dev = mapper_db_get_device_by_name("/testdb.3");
    if (!dev) {
        printf("Not found.\n");
        return 1;
    }

    printf("  name=%s, host=%s, port=%d, canAlias=%d\n",
           dev->name, dev->host, dev->port, dev->canAlias);

    /*********/

    printf("\nFind /dummy:\n");

    dev = mapper_db_get_device_by_name("/dummy");
    if (dev) {
        printf("unexpected found /dummy: %p\n", dev);
        return 1;
    }
    printf("  not found, good.\n");

    /*********/

    printf("\nFind matching '__':\n");

    pdev = mapper_db_match_device_by_name("__");

    count=0;
    if (!pdev) {
        printf("mapper_db_match_device_by_name() returned 0.\n");
        return 1;
    }
    if (!*pdev) {
        printf("mapper_db_match_device_by_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (pdev) {
        count ++;
        printf("  name=%s, host=%s, port=%d, canAlias=%d\n",
               (*pdev)->name, (*pdev)->host,
               (*pdev)->port, (*pdev)->canAlias);
        pdev = mapper_db_device_next(pdev);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\n--- Signals ---\n");

    printf("\nFind all inputs for device '/testdb.1':\n");

    mapper_db_signal *psig =
        mapper_db_get_inputs_by_device_name("/testdb.1");

    count=0;
    if (!psig) {
        printf("mapper_db_get_inputs_by_device_name() returned 0.\n");
        return 1;
    }
    if (!*psig) {
        printf("mapper_db_get_inputs_by_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (psig) {
        count ++;
        printsignal(*psig);
        psig = mapper_db_signal_next(psig);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind all outputs for device '/testdb.1':\n");

    psig = mapper_db_get_outputs_by_device_name("/testdb.1");

    count=0;
    if (!psig) {
        printf("mapper_db_get_outputs_by_device_name() returned 0.\n");
        return 1;
    }
    if (!*psig) {
        printf("mapper_db_get_outputs_by_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (psig) {
        count ++;
        printsignal(*psig);
        psig = mapper_db_signal_next(psig);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind all inputs for device '/testdb.2':\n");

    psig = mapper_db_get_inputs_by_device_name("/testdb.2");

    count=0;
    if (psig) {
        printf("mapper_db_get_inputs_by_device_name() "
               "incorrectly found something.\n");
        printsignal(*psig);
        return 1;
    }
    else
        printf("  correctly returned 0.\n");

    /*********/

    printf("\nFind all outputs for device '/testdb.2':\n");

    psig = mapper_db_get_outputs_by_device_name("/testdb.2");

    count=0;
    if (!psig) {
        printf("mapper_db_get_outputs_by_device_name() returned 0.\n");
        return 1;
    }
    if (!*psig) {
        printf("mapper_db_get_outputs_by_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (psig) {
        count ++;
        printsignal(*psig);
        psig = mapper_db_signal_next(psig);
    }

    if (count != 1) {
        printf("Expected 1 record, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind matching input 'in' for device '/testdb.1':\n");

    psig = mapper_db_match_inputs_by_device_name("/testdb.1", "in");

    count=0;
    if (!psig) {
        printf("mapper_db_match_inputs_by_device_name() returned 0.\n");
        return 1;
    }
    if (!*psig) {
        printf("mapper_db_match_inputs_by_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (psig) {
        count ++;
        printsignal(*psig);
        psig = mapper_db_signal_next(psig);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind matching output 'out' for device '/testdb.1':\n");

    psig = mapper_db_match_outputs_by_device_name("/testdb.1", "out");

    count=0;
    if (!psig) {
        printf("mapper_db_match_outputs_by_device_name() returned 0.\n");
        return 1;
    }
    if (!*psig) {
        printf("mapper_db_match_outputs_by_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (psig) {
        count ++;
        printsignal(*psig);
        psig = mapper_db_signal_next(psig);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind matching output 'out' for device '/testdb.2':\n");

    psig = mapper_db_match_outputs_by_device_name("/testdb.2", "out");

    count=0;
    if (!psig) {
        printf("mapper_db_match_outputs_by_device_name() returned 0.\n");
        return 1;
    }
    if (!*psig) {
        printf("mapper_db_match_outputs_by_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (psig) {
        count ++;
        printsignal(*psig);
        psig = mapper_db_signal_next(psig);
    }

    if (count != 1) {
        printf("Expected 1 record, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\n--- Links ---\n");

    printf("\nFind matching links with source '/testdb__.2':\n");

    mapper_db_link* plink =
        mapper_db_get_links_by_source_device_name("/testdb__.2");

    count=0;
    if (!plink) {
        printf("mapper_db_get_links_by_source_device_name() returned 0.\n");
        return 1;
    }
    if (!*plink) {
        printf("mapper_db_get_links_by_source_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (plink) {
        count ++;
        printf("  source=%s, dest=%s\n",
               (*plink)->src_name, (*plink)->dest_name);
        plink = mapper_db_link_next(plink);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind matching links with destination '/testdb__.4':\n");

    plink = mapper_db_get_links_by_dest_device_name("/testdb__.4");

    count=0;
    if (!plink) {
        printf("mapper_db_get_links_by_dest_device_name() returned 0.\n");
        return 1;
    }
    if (!*plink) {
        printf("mapper_db_get_links_by_dest_device_name() returned something "
               "which pointed to 0.\n");
        return 1;
    }

    while (plink) {
        count ++;
        printf("  source=%s, dest=%s\n",
               (*plink)->src_name, (*plink)->dest_name);
        plink = mapper_db_link_next(plink);
    }

    if (count != 1) {
        printf("Expected 1 record, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind links with source matching 'db' and "
           "destination matching '__':\n");

    pdev = mapper_db_match_device_by_name("db");

    if (!pdev) {
        printf("mapper_db_match_device_by_name() returned 0.\n");
        return 1;
    }

    mapper_db_device_t **pdev2 = mapper_db_match_device_by_name("__");

    if (!pdev2) {
        printf("mapper_db_match_device_by_name() returned 0.\n");
        return 1;
    }

    plink = mapper_db_get_links_by_source_dest_devices(pdev, pdev2);

    count=0;
    if (!plink) {
        printf("mapper_db_get_links_by_source_dest_devices() returned 0.\n");
        return 1;
    }
    if (!*plink) {
        printf("mapper_db_get_links_by_source_dest_devices() "
               "returned something which pointed to 0.\n");
        return 1;
    }

    while (plink) {
        count ++;
        printf("  source=%s, dest=%s\n",
               (*plink)->src_name, (*plink)->dest_name);
        plink = mapper_db_link_next(plink);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nFind any links with source matching '2':\n");

    pdev = mapper_db_match_device_by_name("2");

    if (!pdev) {
        printf("mapper_db_match_device_by_name() returned 0.\n");
        return 1;
    }

    pdev2 = mapper_db_get_all_devices();

    if (!pdev2) {
        printf("mapper_db_get_all_devices() returned 0.\n");
        return 1;
    }

    plink = mapper_db_get_links_by_source_dest_devices(pdev, pdev2);

    count=0;
    if (!plink) {
        printf("mapper_db_get_links_by_source_dest_devices() returned 0.\n");
        return 1;
    }
    if (!*plink) {
        printf("mapper_db_get_links_by_source_dest_devices() "
               "returned something which pointed to 0.\n");
        return 1;
    }

    while (plink) {
        count ++;
        printf("  source=%s, dest=%s\n",
               (*plink)->src_name, (*plink)->dest_name);
        plink = mapper_db_link_next(plink);
    }

    if (count != 2) {
        printf("Expected 2 records, but counted %d.\n", count);
        return 1;
    }

    /*********/

    printf("\nTest PASSED.\n");
    return 0;
}