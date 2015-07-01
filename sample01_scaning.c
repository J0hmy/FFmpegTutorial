#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <stdio.h>

static AVFormatContext* fmt_ctx = NULL;

static int open_input(const char* filename)
{
	unsigned int index;
	int ret;

	// 주어진 파일 이름으로부터 fmt_ctx를 가져옵니다. 
	ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL);
	if(ret < 0)
	{
		printf("Could not open input file %s\n", filename);
		return -1;
	}

	//주어진 fmt_ctx로부터 유효한 스트림이 있는지 찾습니다.
	ret = avformat_find_stream_info(fmt_ctx, NULL);
	if(ret < 0)
	{
		printf("Failed to retrieve input stream information\n");
		return -2;
	}

	// fmt_ctx->nb_streams : 컨테이너가 저장하고 있는 총 스트림 갯수
	for(index = 0; index < fmt_ctx->nb_streams; index++)
	{
		AVCodecContext* avCodecContext = fmt_ctx->streams[index]->codec;
		if(avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			printf("------- Video info -------\n");
			printf("codec_id : %d\n", avCodecContext->codec_id);
			printf("bitrate : %d\n", avCodecContext->bit_rate);
			printf("width : %d / height : %d\n", avCodecContext->width, avCodecContext->height);
		}
		else if(avCodecContext->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			printf("------- Audio info -------\n");
			printf("codec_id : %d\n", avCodecContext->codec_id);
			printf("bitrate : %d\n", avCodecContext->bit_rate);
			printf("sample_rate : %d\n", avCodecContext->sample_rate);
			printf("number of channels : %d\n", avCodecContext->channels);
		}
	} // for

	return 0;
}

static void release()
{
	if(fmt_ctx != NULL)
	{
		avformat_close_input(&fmt_ctx);
	}
}

int main(int argc, char* argv[])
{
	int ret;

	av_register_all();

	// FFmpeg 라이브러리 레벨에서 디버깅 로그를 출력하도록 합니다.
	av_log_set_level(AV_LOG_DEBUG);

	if(argc < 2)
	{
		printf("usage : %s <input>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	ret = open_input(argv[1]);
	if(ret < 0)
	{
		release();
		exit(EXIT_SUCCESS);
	}

	release();
	return 0;
}