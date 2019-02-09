#include <assert.h>
#include <nfastapp.h>
#include <seelib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "rand.h"


NFastApp_Connection conn;
NFast_AppHandle app;
M_CertificateList cert_list;
M_Certificate certs[10];
static bool initialized = false;

/**
 * Connect to the nCipher.
 *
 * returns -1 if nCipher init fails.
 * returns -2 if nCipher connect fails.
 */
int init() {
  NFastAppInitArgs ia = {0};
  M_Status rc;

  if ((rc = NFastApp_InitEx(&app, &ia, NULL))) {
    fprintf(stderr, "NFastApp_InitEx failed: %s", NF_Lookup(rc, NF_Status_enumtable));
    return -1;
  }

  if ((rc = NFastApp_Connect(app, &conn, 0, NULL))) {
    fprintf(stderr, "NFastApp_Connect failed: %s", NF_Lookup(rc, NF_Status_enumtable));
    NFastApp_Finish(app, NULL);
    return -2;
  }

  /* Result r = module_certificate_init(&cert_list, certs); */
  /* if (r != Result_SUCCESS) { */
  /*   fprintf(stderr, "get_module_certificate_list failed (%d)", r); */
  /*   NFastApp_Finish(app, NULL); */
  /*   return -3; */
  /* } */

  initialized = true;
  return 0;
}

void cleanup() {
  if (initialized) {
    NFastApp_Disconnect(conn, NULL);
    NFastApp_Finish(app, NULL);
    initialized = false;
    //module_certificate_cleanup(&cert_list);
  }
}

uint8_t gen_random_buffer[4];
float get_rand() {
  M_Status retcode;
  M_Command command = {0};
  M_Reply reply = {0};

  command.cmd = Cmd_GenerateRandom;
  command.args.generaterandom.lenbytes = sizeof(gen_random_buffer);
  command.certs = &cert_list;
  command.flags |= Command_flags_certs_present;

  if ((retcode = NFastApp_Transact(conn, NULL, &command, &reply, NULL)) !=
      Status_OK) {
    fprintf(stderr, "NFastApp_Transact failed");
    return 0.0;
  }
  if ((retcode = reply.status) != Status_OK) {
    fprintf(stderr, "NFastApp_Transact not ok\n");
    char buf[1000];
    NFast_StrError(buf, sizeof(buf), reply.status, NULL);
    fprintf(stderr, "message: %s", buf);

    NFastApp_Free_Reply(app, NULL, NULL, &reply);
    return 0.0;
  }

  if (reply.reply.generaterandom.data.len != sizeof(gen_random_buffer)) {
    fprintf(stderr, "invalid data len");
    NFastApp_Free_Reply(app, NULL, NULL, &reply);
    return 0.0;
  }
  float f = (float)(*(uint32_t*)reply.reply.generaterandom.data.ptr) / 0xffffffff;
  NFastApp_Free_Reply(app, NULL, NULL, &reply);

  return f;
}
