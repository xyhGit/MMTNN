//
//  frame-data.h
//  ndnrtc
//
//  Copyright 2013 Regents of the University of California
//  For licensing details see the LICENSE file.
//
//  Author:  Peter Gusev
//

#ifndef __frame_data__
#define __frame_data__

#include <ndn-cpp/name.hpp>

#include "common.h"


/**
   *@brief This stucture is used for storing meta info carried by data name prefixes
   *@param
  */
struct PrefixMetaInfo {
    SegmentNumber totalSegmentNum_;     // total number of segments for this frame
    PacketNumber playbackNo_;           // absolute plaback positon for current frame
    FrameNumber deltaFrameNo_;          // sequence number of the corresponding key frame
};


class BaseData {

public:
    BaseData() : length_(0), data_(NULL) {}
    BaseData(unsigned int length, const unsigned char *rawData );
    BaseData( const BaseData &mediaData );

    virtual ~BaseData();

    int size() const { return length_; }
    unsigned char *buf() const { return data_; }

    virtual int
    initFromRawData(unsigned int dataLength,
                    const unsigned char *rawData) = 0;

    void copyFromRaw(unsigned int dataLength,
                     const unsigned char *rawData);

protected:

    unsigned int length_;
    unsigned char *data_ = NULL;
    bool isDataCopied_ = false;

};


class SegmentData : public BaseData {

public:
    typedef struct SegmentMetaInfo {
        uint32_t interestNonce_;
        int64_t interestArrivalMs_;
        int64_t generationDelayMs_;
    } __attribute__((packed)) SegmentMetaInfo ;

    SegmentData(){}
    SegmentData( const unsigned char *segmentData, const unsigned int dataSize,
                 SegmentMetaInfo metadata = (SegmentMetaInfo){0,0,0} );

    SegmentMetaInfo *getMetaData() { return ((SegmentMetaInfo*)(&data_[0])); }

    unsigned char *getSegmentData() { return data_ + getHeaderSize(); }

    unsigned int getHeaderSize() { return sizeof(SegmentMetaInfo); }

    unsigned int getSegmentDataLenth() { return length_ - getHeaderSize(); }

    int initFromRawData(unsigned int dataLength,
                        const unsigned char *rawData);

    static int segmentDataFromRaw(unsigned int dataLength,
                                   const unsigned char *rawData,
                                   SegmentData &segmentData);

private:
    typedef SegmentMetaInfo SegmentHeader;
};


//*****************************************************
//  Base media data, like video and audio
//*****************************************************

class MediaData : public BaseData {
public:
    enum MediaDataType {
        TypeVideo = 1,
        TypeAudio = 2,
        TypeParity = 3
    };
/*
    struct Metadata {
        double packetRate_; // current packet production rate
        int64_t timestamp_; // packet timestamp set by producer
        double unixTimestamp_; // unix timestamp set by producer
    } __attribute__((packed));
*/
    MediaData(){}

    MediaData( unsigned int dataLength, const unsigned char *data ) :
        BaseData( dataLength, data ) {}

//    virtual Metadata getMetadata() = 0;

//    virtual void setMetadata(Metadata &metadata) = 0;

    virtual MediaDataType getType() = 0;

};


class FrameData : public MediaData {

public:

    struct FrameMetadata {
        uint32_t        encodedWidth_;
        uint32_t        encodedHeight_;
        double packetRate_; // current packet production rate
        int64_t timestamp_; // packet timestamp set by producer
        double unixTimestamp_; // unix timestamp set by producer
    } __attribute__((packed));

    FrameData(){}
    FrameData( unsigned char *data, unsigned int length,
               FrameMetadata *metaData );

    MediaDataType getType() { return TypeVideo;}

    unsigned int getHeaderSize() { return sizeof( FrameHeader); }

    FrameMetadata *getMetadata() { return (FrameMetadata*)(&data_[0]); }

    unsigned int getDataSize() { return length_ - sizeof( FrameHeader); }

    unsigned char *getFrameData() { return (unsigned char*)(&data_[getHeaderSize()]);}

    static int packetFromRaw(unsigned int length, unsigned char *data,
                          FrameData *frameData);

    int initFromRawData(unsigned int dataLength, const unsigned char *rawData);

protected:

    typedef FrameMetadata FrameHeader;
};


class RGBFrameData : public MediaData
{
public:
    RGBFrameData(const unsigned int width, const unsigned int height,
                 const unsigned char *rawData, const unsigned int length);
    ~RGBFrameData();

    const unsigned int width() const
    { return width_; }

    const unsigned int height() const
    { return height_; }

    const unsigned int size() const
    { return length_; }

    const unsigned char *data() const
    { return data_; }

private:
    unsigned int width_;
    unsigned int height_;
};


//*****************************************************
//  Old struct
//*****************************************************

//struct FrameDataHeader {
//        uint32_t					frameNumber;
//        uint32_t                    encodedWidth_;
//        uint32_t                    encodedHeight_;
//        uint32_t					length_;			// data length (without header)
//        uint32_t                    timeStamp_;
//        FrameType					frameType_;
//        bool                        completeFrame_;
//} __attribute__((packed));

//struct FrameData {
//    FrameDataHeader header_;
//    unsigned char *	buf_;
//};


#endif /*defined(__frame_data__) */
