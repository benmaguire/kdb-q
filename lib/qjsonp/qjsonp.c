#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <json/json.h>
#include "k.h"


const long long timediff = 9.466848e+14; //946684800;


// Close
K q_parse_tick (K tick_k)
{
        if (tick_k->t != KC) return krr("type");

        char *tick = malloc(sizeof(char) * (tick_k->n+1));
        if (tick == NULL) {
            free(tick);
            return krr("wsfull");
        }
                
        tick[tick_k->n] = '\0';
        memcpy(tick, kC(tick_k), tick_k->n);
        
        json_object *jobj_root;
        int exists;
        
        jobj_root = json_tokener_parse(tick);
        if(jobj_root == NULL) {
            json_object_put(jobj_root);
            return krr("json parse error");
        }
        
        json_object *jobj_tick, *jobj_1, *jobj_2, *jobj_3, *jobj_4;
        
        // JSON Root
        exists=json_object_object_get_ex(jobj_root,"tick",&jobj_tick);
        if(exists==false) {
            json_object_put(jobj_root);
            json_object_put(jobj_tick);
            json_object_put(jobj_1);
            json_object_put(jobj_2);
            json_object_put(jobj_3);
            json_object_put(jobj_4);
            free(tick);
            return krr("json tick error");
        }


        K inst_k,bid_k,ask_k,time_k,c,v;
        
        c=ktn(KS,4);kS(c)[0]=ss("time");kS(c)[1]=ss("bid");kS(c)[2]=ss("ask");kS(c)[3]=ss("instrument");
        

        // JSON Instrument
        exists=json_object_object_get_ex(jobj_tick,"instrument",&jobj_1);
        inst_k = ktn(KS,1);
        kS(inst_k)[0]=ss(json_object_get_string(jobj_1));
        
        // JSON Bid
        exists=json_object_object_get_ex(jobj_tick,"bid",&jobj_2);
        bid_k = ktn(KF,1);
        kF(bid_k)[0]=json_object_get_double(jobj_2);
        
        // JSON Ask
        exists=json_object_object_get_ex(jobj_tick,"ask",&jobj_3);
        ask_k = ktn(KF,1);
        kF(ask_k)[0]=json_object_get_double(jobj_3);
        
        // JSON Date
        exists=json_object_object_get_ex(jobj_tick,"time",&jobj_4);
        time_k = ktn(KP,1);
        
        long long convertime;
        convertime = atoll(json_object_get_string(jobj_4));
        //convertime = atof(json_object_get_string(jobj_4));
        //kF(time_k)[0]=((convertime/1000000)-timediff)/86400;
        kJ(time_k)[0]=(convertime-timediff)*1000;
        
        v=knk(4,time_k,inst_k,bid_k,ask_k);
        //val=xT(xD(c,v));


        json_object_put(jobj_root);
        json_object_put(jobj_tick);
        json_object_put(jobj_1);
        json_object_put(jobj_2);
        json_object_put(jobj_3);
        json_object_put(jobj_4);
        free(tick);
        
        
        return v;
        

}
