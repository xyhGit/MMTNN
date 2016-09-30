#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <ndn-cpp/name.hpp>
#include <ndn-cpp/face.hpp>
#include <ndn-cpp/security/key-chain.hpp>

#include "name-components.h"
#include "frame-buffer.h"
#include "capturer.hpp"
#include "encoder.hpp"

using namespace ndn;

class FrameBuffer;

class Publisher
{
public:

    Publisher ( KeyChain &keyChain, const Name& certificateName );

    ~Publisher ();

    bool init ();

    int start();

    int stop();

    int getStatus()
    { return stat; }

    bool isCached( FrameNumber frameNo )
    { return (frameNo >= cachedBegin_ && frameNo <= cachedEnd_) ? true : false; }

    // onInterest.
    void operator()
                    (const ptr_lib::shared_ptr<const Name>& prefix,
                    const ptr_lib::shared_ptr<const Interest>& interest, Face& face,
                    uint64_t interestFilterId,
                    const ptr_lib::shared_ptr<const InterestFilter>& filter);

    // onRegisterFailed.
    void operator()(const ptr_lib::shared_ptr<const Name>& prefix);


    ndn::Name getStreamVideoPrefix()
    { return streamPrefix_.append(NameComponents::NameComponentStreamFrameVideo); }

    ndn::Name getStreamPrefix()
    { return streamPrefix_; }

    std::string getLocation()
    { return location_; }

    int getStreamIdx()
    { return stream_; }


private:

    void excuteCapture();

    void view();

    ndn::Name       streamPrefix_;
    std::string     location_;
    int             stream_;

    KeyChain        keyChain_;
    Name            certificateName_;
    int             requestCounter_,
                    responseCount_;

    int stat;

    boost::shared_ptr<FrameBuffer> framebuffer_;

    Capturer capturer;
    Encoder encoder;

    int width = 640, height = 480;

    unsigned char* outbufYUV = NULL;
    int outlenYUV = 0;
    unsigned char* outbuf264 = NULL;
    int outlen264 = 0;

    bool isBackupYUV, isBackup264;
    FILE *fp_yuv, *fp_264;

    uint64_t    cacheSize_,
                cachedBegin_,
                cachedEnd_;
};


#endif // PUBLISHER_H
