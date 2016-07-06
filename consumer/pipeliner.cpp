/*
 * pipeliner.cpp
 *
 *  Created on: Jun 15, 2016
 *      Author: xyh
 */

#include <iostream>
#include "pipeliner.h"

using namespace std;


Pipeliner::Pipeliner(boost::shared_ptr<FrameBuffer> frameBuffer):
	frameBuffer_(frameBuffer)
{
	pipelinerFIle_ = fopen ( "pipelinerFIle_.264", "wb+" );
	if ( pipelinerFIle_ == NULL )
	{
		std::cout << "open consumer.yuv error" << std::endl;
		return;
	}
}

Pipeliner::~Pipeliner()
{
	fclose(pipelinerFIle_);
}


void Pipeliner::onData(const ptr_lib::shared_ptr<const Interest>& interest,
		const ptr_lib::shared_ptr<Data>& data)
{
	//std::cout<<"Pipeliner onData:"<<std::endl;

	string framenoStr = data->getName().get(1).toEscapedString();
	long frameNo = std::atoi(framenoStr.c_str());

	if ( data->getContent ().buf () == NULL )
			cout << "content is null !" << endl;
	cout << "Got frame "<< frameNo//data->getName().toUri()
		 << " size: " << data->getContent ().size () << endl;

	frameBuffer_->addFrame( data );


	//fwrite( data->getContent().buf(), data->getContent ().size (),1,pipelinerFIle_);
//	FrameBuffer::Slot *slot = frameBuffer_->getFrame();
//	cout << "size:";
//	cout <<  slot->getFrameSize()<<endl;
//	fwrite( slot->getDataPtr(), slot->getFrameSize(),1,pipelinerFIle_);

	/*
	frame_buf tmpbuf;
	memcpy ( &( tmpbuf.p_In_Frame ), data->getContent ().buf (), data->getContent ().size () );
	tmpbuf.size = data->getContent ().size ();

	cout << "write ring buffer... ";
	// write ring buffer
	while ( 0 != pthread_mutex_lock ( &recv_buf_mutex ) );
	recv_buf->Push ( tmpbuf  );
	cout << recv_buf->size () << " ";
	while ( 0 != pthread_mutex_unlock ( &recv_buf_mutex ) );
	cout << " done." << endl;

	while ( 0 != pthread_mutex_lock ( &recv_buf_mutex ) );
	frame_buf ptmpbuf = recv_buf->Pop ();
	while ( 0 != pthread_mutex_unlock ( &recv_buf_mutex ) );

	cout <<"size: " << ptmpbuf.size << endl;
	fwrite ( &(ptmpbuf.p_In_Frame), ptmpbuf.size, 1, pf );
	 */
	//cout << endl;
}


void Pipeliner::onTimeout(const ptr_lib::shared_ptr<const Interest>& interest)
{
	cout << "Pipeliner timeout" << endl;
}
