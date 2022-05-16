#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <zookeeper/zookeeper.h>
#include "k.h"


#define CONNECTOR_PATH "/fts/service/connector"
#define CALLBACK ".zk.ps"

static zhandle_t *zh;
static int connected;
char gpath[1024];




void cb()
{
        K cb = ks(CALLBACK);
        K msg = ks("ZK State Change");
        K ignored = k(0, cb->s, r1(msg), (K)0);
        r0(ignored);
        //return (K)0;
}



void watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx) {
        if (type == ZOO_CHILD_EVENT && strncmp(path, CONNECTOR_PATH, sizeof(CONNECTOR_PATH) - 1) == 0)
        {
            cb();
        }

        if (type == ZOO_SESSION_EVENT) {                               
            if (state == ZOO_CONNECTED_STATE) {                        
                connected = 1;                                         
            } else if (state == ZOO_AUTH_FAILED_STATE) {               
                connected = 0;                                               
            } else if (state == ZOO_EXPIRED_SESSION_STATE) {           
                connected = 0;                                       
            }
        }

}



// Connect
K q_zk_connect (K zkCS_k)
{
        //if (zkHost->t != -KJ || zkPort->t != KC) return krr("type");
        
        //char mqcstr[50];
        //strcpy(mqcstr, zkHost);
        //strcat(mqcstr, zkPort);
        const char* zkCS = zkCS_k->s;
        
        zoo_deterministic_conn_order(1);
        zh = zookeeper_init(zkCS, watcher, 30000, 0, 0, 0);
        if (!zh) {
            return krr("zookeeper init failed");
        }
        return (K)0;
}

// Close
K q_zk_disconnect ()
{
	zookeeper_close(zh);
	return (K)0;
}



// zoo_create
K q_zk_createservice (K csdata_k, K csname_k)
{
        const char* csdata = csdata_k->s;
        const char* csname = csname_k->s;
        int rc = zoo_create(zh, csname, csdata, 20, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL|ZOO_SEQUENCE, gpath, 1023);

        if(rc != ZOK) {
            return krr("zookeeper create failed");
        }
        return (K)0;
}



// get zmq
K q_zk_getzmq()
{
        
        struct String_vector children;
        int rc = zoo_get_children(zh, CONNECTOR_PATH, 1, &children);
        
        if(rc != ZOK) {
            return krr("zookeeper failed");
        }

        if(children.count < 1) {
            return krr("no zmq found");
        }
        
        
        char path[255];
        sprintf(path, "/fts/service/connector/%s", children.data[0]);

        char buffer[32];
        int buflen= sizeof(buffer);
        
        zoo_get(zh, path, 0, buffer, &buflen, NULL);          
        
        
        K zmqdata = ks(buffer);
        return zmqdata;


}



// get kxtick
K q_zk_getkxtick()
{

        struct String_vector children;
        int rc = zoo_get_children(zh, CONNECTOR_PATH, 1, &children);

        if(rc != ZOK) {
            return krr("zookeeper failed");
        }

        if(children.count < 1) {
            return krr("no kxtick found");
        }


        char path[255];
        sprintf(path, "/fts/service/kdb/tick/%s", children.data[0]);

        char buffer[32];
        int buflen= sizeof(buffer);

        zoo_get(zh, path, 0, buffer, &buflen, NULL);


        K zmqdata = ks(buffer);
        return zmqdata;


}


