#include <stdio.h>
#include "pipeline_manager.h"

extern void test_V1_pipeline(pipeline_manager* manager);

typedef struct _pipeline_collection {
    const char* pipeline_name;
    void(*f)(pipeline_manager* manager);
}pipeline_collection;

pipeline_collection pipelines[] = {
    {"test_V1_pipeline", test_V1_pipeline}
};

// -p pipe_number -cfg xml_file -f frame_end 
int main(int argc, char* argv[])
{
    if (argc < 7) {
        fprintf(stdout, "input param number must >= 5\n");
        return 0;
    }
    fprintf(stdout, "run command:");
    for (int i = 0; i < argc; i++)
    {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");

    int32_t pipe_id = -1;
    char* cfg_file_name = NULL;
    int32_t frame_end = -1;

    for (int32_t i = 1; i < argc; i++)
    {
        if (strcmp("-p", argv[i]) == 0 && i < (argc - 1))
        {
            pipe_id = atoi(argv[i + 1]);
        }
        if (strcmp("-cfg", argv[i]) == 0 && i < (argc - 1))
        {
            cfg_file_name = argv[i + 1];
        }
        if (strcmp("-f", argv[i]) == 0 && i < (argc - 1))
        {
            frame_end = atoi(argv[i + 1]);
        }
    }
    if (pipe_id < 0 || frame_end < 0 || cfg_file_name == NULL)
    {
        fprintf(stdout, "param error\n");
        return 0;
    }

    if (pipe_id >= sizeof(pipelines) / sizeof(pipeline_collection))
    {
        fprintf(stdout, "pipe_id out of range\n");
        return 0;
    }

    pipeline_manager isp_pipe_manager;
    pipelines[pipe_id].f(&isp_pipe_manager);

    isp_pipe_manager.init();
    isp_pipe_manager.read_xml_cfg(cfg_file_name);

    return 0;
}