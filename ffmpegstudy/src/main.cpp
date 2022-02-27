#include <iostream>
extern "C" {

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}
using namespace std;

int main()
{
    cout << "ffmpeg study!" << endl;

    char* fileName = "road.mp4";
    AVOutputFormat* avFormat = av_guess_format( 0, fileName, 0 );
    cout << "avformat: " << avFormat <<endl;
    if( !avFormat )
    {
        cerr << "Can't guess format of file:" << fileName << endl;
        return 0;
    }

    cout << "video codecId:" << avFormat->video_codec << " audioCodec:" << avFormat->audio_codec << endl;

    AVFormatContext *ifmt_ctx = NULL;
    //Input
    int ret = 0;
	if (( ret = avformat_open_input(&ifmt_ctx, fileName, 0, 0)) < 0) {
		printf( "Could not open input file.");
		return 0;
	}

    if (avformat_find_stream_info(ifmt_ctx, 0) < 0)
	{
		printf("Call av_find_stream_info function failed!\n");
		return 0;
	}
	//输出视频信息
	av_dump_format(ifmt_ctx, -1, fileName, 0);

    for( int i=0; i<ifmt_ctx->nb_streams; ++i)
    {
        AVCodecID codecId = ifmt_ctx->streams[i]->codecpar->codec_id;
        AVMediaType mediaType = ifmt_ctx->streams[i]->codecpar->codec_type;
        cout << "stream " << i << ":\t" << "codecId:" << codecId << " mediaType:" << mediaType << "(0:video;1:audio;)" << endl;
    }

    AVFormatContext* ofmtctx = avformat_alloc_context();         

     //设置流格式为RTP                                   
    ofmtctx->oformat = av_guess_format("rtp", NULL, NULL);   
    //用指定IP和端口构造输出流地址
    sprintf(ofmtctx->filename,"rtp://%s:%d", "192.168.84.153",8000);

    //打开输出流
    avio_open(&ofmtctx->pb,fmtctx->filename, AVIO_FLAG_WRITE);

    AVPacket* pkt = av_packet_alloc();
    while(true)
    {
        int err = av_read_frame( ifmt_ctx, pkt );
        if( err != 0 )
        {
            cout << "av_read_frame end!" << endl;
            break;
        }
        else
        {
            cout << "read frame, stream:" << pkt->stream_index << " size:" << pkt->size << endl;
        }
    }

    av_packet_free( &pkt );
    //av_register_all();

    return 0;
}