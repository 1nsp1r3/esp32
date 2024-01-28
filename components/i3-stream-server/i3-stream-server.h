#ifndef I3_STREAM_SERVER_H
#define I3_STREAM_SERVER_H

#define I3_STREAM_SERVER_TAG "I3-STREAM-SERVER"

#include <i3-httpd.h>

#define MULTIPART_BOUNDARY "Inspir3"
#define MULTIPART "multipart/x-mixed-replace;boundary=" MULTIPART_BOUNDARY

class I3StreamServer: public I3Httpd {
  public:
    I3StreamServer():I3Httpd(){}
    void addStreamEndpoint(const char* Path, GetCallBack GetCallBack);
    bool sendPart(httpd_req_t* Req, I3HttpdContent* Content);
};

#endif
