#include <iostream>
extern "C" {

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>

int64_t 	av_gettime (void);
int 	av_usleep (unsigned usec);
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
    sprintf(ofmtctx->filename,"rtp://%s:%d", "192.168.84.90",8000);

    //打开输出流
    avio_open(&ofmtctx->pb,ofmtctx->filename, AVIO_FLAG_WRITE);

    ifmt_ctx->nb_streams;
    ifmt_ctx->video_codec;
    ifmt_ctx->audio_codec;
    //avformat_new_stream( ofmtctx, ifmt_ctx->nb_streams )
    cout << "out nb_streams before create:" << ofmtctx->nb_streams << endl;
    
    auto vstream = avformat_new_stream( ofmtctx, NULL );
    if( vstream == 0 )
    {
        cout << "avformat_new_stream for out fail!!" << endl;
    }
    cout << "out nb_streams:" << ofmtctx->nb_streams << endl;

    auto inVStream = ifmt_ctx->streams[0];
    if( vstream )
    {
        // 从 input stream 中拷贝codec参数.
        vstream->time_base = inVStream->time_base;
        cout << "if time_base:[" << vstream->time_base.num << "/" << vstream->time_base.den << "]" << endl;
        vstream->sample_aspect_ratio = inVStream->sample_aspect_ratio;
        avcodec_parameters_copy( vstream->codecpar, inVStream->codecpar );
    

        // H264,把时间戳的基准改掉.
        vstream->time_base.num = 1;
        vstream->time_base.den = 90000;
    
    }
    
    ret = avformat_write_header(ofmtctx, NULL);
    cout << "avformat_write_header ret:" << ret << endl;

    auto start_time = av_gettime();

    AVPacket* pkt = av_packet_alloc();
    while(true)
    {
        int err = av_read_frame( ifmt_ctx, pkt );
        if( err != 0 )
        {
            cout << "av_read_frame end!" << endl;
            // flush, 输出缓存的数据.
            av_write_frame(ofmtctx, NULL );
            break;
        }
        else
        {

            // 只输出视频
            if( pkt->stream_index == 0 )
            {
                cout << "frame, stream:" << pkt->stream_index << " size:" << pkt->size << " if pts:" << pkt->pts << " dts: " << pkt->dts;

                // pkt的时间基准不一样,做转换.
                pkt->pts = av_rescale_q(pkt->pts, inVStream->time_base,  vstream->time_base);
                pkt->dts = av_rescale_q(pkt->dts, inVStream->time_base,  vstream->time_base);

                cout << " of pts:" << pkt->pts << " dts: " << pkt->dts << endl;

                // 延时处理.
                auto time_base_sys = AV_TIME_BASE_Q; //{1, AV_TIME_BASE };
                auto dts_time = av_rescale_q( pkt->dts, vstream->time_base, time_base_sys );
                auto now_time = av_gettime() - start_time;
                if( dts_time > now_time )
                {
                    av_usleep( dts_time - now_time );
                }

                av_write_frame( ofmtctx, pkt );
            }
        }
    }

    av_packet_free( &pkt );
    //av_register_all();

    return 0;
}