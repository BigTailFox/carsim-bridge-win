// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <string.h>
#include "carsim_RoadQuery.h"

static int __carsim_RoadQuery_hash_computed;
static uint64_t __carsim_RoadQuery_hash;

uint64_t __carsim_RoadQuery_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __carsim_RoadQuery_get_hash)
            return 0;

    __lcm_hash_ptr cp;
    cp.parent =  p;
    cp.v = __carsim_RoadQuery_get_hash;
    (void) cp;

    uint64_t hash = (uint64_t)0x71776515d002b752LL
         + __carsim_QueryPoint_hash_recursive(&cp)
         + __carsim_QueryPoint_hash_recursive(&cp)
         + __carsim_QueryPoint_hash_recursive(&cp)
         + __carsim_QueryPoint_hash_recursive(&cp)
        ;

    return (hash<<1) + ((hash>>63)&1);
}

int64_t __carsim_RoadQuery_get_hash(void)
{
    if (!__carsim_RoadQuery_hash_computed) {
        __carsim_RoadQuery_hash = (int64_t)__carsim_RoadQuery_hash_recursive(NULL);
        __carsim_RoadQuery_hash_computed = 1;
    }

    return __carsim_RoadQuery_hash;
}

int __carsim_RoadQuery_encode_array(void *buf, int offset, int maxlen, const carsim_RoadQuery *p, int elements)
{
    int pos = 0, element;
    int thislen;

    for (element = 0; element < elements; element++) {

        thislen = __carsim_QueryPoint_encode_array(buf, offset + pos, maxlen - pos, &(p[element].left_front), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __carsim_QueryPoint_encode_array(buf, offset + pos, maxlen - pos, &(p[element].left_rear), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __carsim_QueryPoint_encode_array(buf, offset + pos, maxlen - pos, &(p[element].right_front), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __carsim_QueryPoint_encode_array(buf, offset + pos, maxlen - pos, &(p[element].right_rear), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int carsim_RoadQuery_encode(void *buf, int offset, int maxlen, const carsim_RoadQuery *p)
{
    int pos = 0, thislen;
    int64_t hash = __carsim_RoadQuery_get_hash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    thislen = __carsim_RoadQuery_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int __carsim_RoadQuery_encoded_array_size(const carsim_RoadQuery *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {

        size += __carsim_QueryPoint_encoded_array_size(&(p[element].left_front), 1);

        size += __carsim_QueryPoint_encoded_array_size(&(p[element].left_rear), 1);

        size += __carsim_QueryPoint_encoded_array_size(&(p[element].right_front), 1);

        size += __carsim_QueryPoint_encoded_array_size(&(p[element].right_rear), 1);

    }
    return size;
}

int carsim_RoadQuery_encoded_size(const carsim_RoadQuery *p)
{
    return 8 + __carsim_RoadQuery_encoded_array_size(p, 1);
}

int __carsim_RoadQuery_decode_array(const void *buf, int offset, int maxlen, carsim_RoadQuery *p, int elements)
{
    int pos = 0, thislen, element;

    for (element = 0; element < elements; element++) {

        thislen = __carsim_QueryPoint_decode_array(buf, offset + pos, maxlen - pos, &(p[element].left_front), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __carsim_QueryPoint_decode_array(buf, offset + pos, maxlen - pos, &(p[element].left_rear), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __carsim_QueryPoint_decode_array(buf, offset + pos, maxlen - pos, &(p[element].right_front), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __carsim_QueryPoint_decode_array(buf, offset + pos, maxlen - pos, &(p[element].right_rear), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int __carsim_RoadQuery_decode_array_cleanup(carsim_RoadQuery *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __carsim_QueryPoint_decode_array_cleanup(&(p[element].left_front), 1);

        __carsim_QueryPoint_decode_array_cleanup(&(p[element].left_rear), 1);

        __carsim_QueryPoint_decode_array_cleanup(&(p[element].right_front), 1);

        __carsim_QueryPoint_decode_array_cleanup(&(p[element].right_rear), 1);

    }
    return 0;
}

int carsim_RoadQuery_decode(const void *buf, int offset, int maxlen, carsim_RoadQuery *p)
{
    int pos = 0, thislen;
    int64_t hash = __carsim_RoadQuery_get_hash();

    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;

    thislen = __carsim_RoadQuery_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int carsim_RoadQuery_decode_cleanup(carsim_RoadQuery *p)
{
    return __carsim_RoadQuery_decode_array_cleanup(p, 1);
}

int __carsim_RoadQuery_clone_array(const carsim_RoadQuery *p, carsim_RoadQuery *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __carsim_QueryPoint_clone_array(&(p[element].left_front), &(q[element].left_front), 1);

        __carsim_QueryPoint_clone_array(&(p[element].left_rear), &(q[element].left_rear), 1);

        __carsim_QueryPoint_clone_array(&(p[element].right_front), &(q[element].right_front), 1);

        __carsim_QueryPoint_clone_array(&(p[element].right_rear), &(q[element].right_rear), 1);

    }
    return 0;
}

carsim_RoadQuery *carsim_RoadQuery_copy(const carsim_RoadQuery *p)
{
    carsim_RoadQuery *q = (carsim_RoadQuery*) malloc(sizeof(carsim_RoadQuery));
    __carsim_RoadQuery_clone_array(p, q, 1);
    return q;
}

void carsim_RoadQuery_destroy(carsim_RoadQuery *p)
{
    __carsim_RoadQuery_decode_array_cleanup(p, 1);
    free(p);
}

int carsim_RoadQuery_publish(lcm_t *lc, const char *channel, const carsim_RoadQuery *p)
{
      int max_data_size = carsim_RoadQuery_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = carsim_RoadQuery_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _carsim_RoadQuery_subscription_t {
    carsim_RoadQuery_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void carsim_RoadQuery_handler_stub (const lcm_recv_buf_t *rbuf,
                            const char *channel, void *userdata)
{
    int status;
    carsim_RoadQuery p;
    memset(&p, 0, sizeof(carsim_RoadQuery));
    status = carsim_RoadQuery_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding carsim_RoadQuery!!!\n", status);
        return;
    }

    carsim_RoadQuery_subscription_t *h = (carsim_RoadQuery_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    carsim_RoadQuery_decode_cleanup (&p);
}

carsim_RoadQuery_subscription_t* carsim_RoadQuery_subscribe (lcm_t *lcm,
                    const char *channel,
                    carsim_RoadQuery_handler_t f, void *userdata)
{
    carsim_RoadQuery_subscription_t *n = (carsim_RoadQuery_subscription_t*)
                       malloc(sizeof(carsim_RoadQuery_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel,
                                 carsim_RoadQuery_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg carsim_RoadQuery LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int carsim_RoadQuery_subscription_set_queue_capacity (carsim_RoadQuery_subscription_t* subs,
                              int num_messages)
{
    return lcm_subscription_set_queue_capacity (subs->lc_h, num_messages);
}

int carsim_RoadQuery_unsubscribe(lcm_t *lcm, carsim_RoadQuery_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr,
           "couldn't unsubscribe carsim_RoadQuery_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}

