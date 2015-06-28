#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <stdio.h>

typedef struct _FileContext
{
	AVFormatContext* avFormatContext;
	const char* fileName;
} FileContext;

static FileContext inputFile;

static int openInputFile(const char* fileName)
{
	AVFormatContext* avFormatContext = inputFile.avFormatContext;
	unsigned int index;
	int returnCode;

	avFormatContext = NULL;
	inputFile.fileName = fileName;
	returnCode = avformat_open_input(&avFormatContext, inputFile.fileName, NULL, NULL);
	if(returnCode < 0)
	{
		fprintf(stderr, "Could not open input file %s\n", inputFile.fileName);
		return -1;
	}

	returnCode = avformat_find_stream_info(avFormatContext, NULL);
	if(returnCode < 0)
	{
		fprintf(stderr, "Failed to retrieve input stream information\n");
		return -2;
	}

	for(index = 0; index < avFormatContext->nb_streams; index++)
	{
		AVCodecContext* avCodecContext = avFormatContext->streams[index]->codec;
		if(avCodecContext->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			fprintf(stdout, "------- Video info -------\n");
			fprintf(stdout, "codec_id : %d\n", avCodecContext->codec_id);
			fprintf(stdout, "bitrate : %d\n", avCodecContext->bit_rate);
			fprintf(stdout, "width : %d / height : %d\n", avCodecContext->width, avCodecContext->height);
		}
		else if(avCodecContext->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			fprintf(stdout, "------- Audio info -------\n");
			fprintf(stdout, "codec_id : %d\n", avCodecContext->codec_id);
			fprintf(stdout, "bitrate : %d\n", avCodecContext->bit_rate);
			fprintf(stdout, "sample_rate : %d\n", avCodecContext->sample_rate);
			fprintf(stdout, "number of channels : %d\n", avCodecContext->channels);
		}
	} // for

	return 0;
}

static void release()
{
	if(inputFile.avFormatContext != NULL)
	{
		avformat_close_input(&inputFile.avFormatContext);
	}
}

int main(int argc, char* argv[])
{
	int returnCode;

	av_register_all();

	if(argc < 2)
	{
		fprintf(stderr, "usage : %s <input>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	returnCode = openInputFile(argv[1]);
	if(returnCode < 0)
	{
		release();
		exit(EXIT_SUCCESS);
	}

	release();
	return 0;
}