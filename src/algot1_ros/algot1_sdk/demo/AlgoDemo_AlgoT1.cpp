#include "AlgoDemo_AlgoT1.h"

#include "uliti/AlgoString.h"
#include "uliti/AlgoCmdLine.h"
#include "uliti/AlgoDirectory.h"
#include "uliti/AlgoCoordTrans.h"

using namespace Algo1010;

int main(int argc, char **argv)
{
    //AlgoCoordTrans::test_AlgoCoordTrans();

    std::cout << "Run AlgoDemo_AlgoT1...\n"
              << argv[0] << std::endl;

    // read or parser config params
    CmdLine cmd;
    std::string m_configJson;
    std::string m_outputPath;
    cmd.add(CmdLine::make_option('i', m_configJson, "config"));
    cmd.add(CmdLine::make_option('o', m_outputPath, "output"));

    try
    {
        cmd.process(argc, argv);

        // check config
        if (!cmd.used('i'))
        {
            std::cout << "No param of config" << std::endl;
            throw std::string("Invalid parameter.");
        }

        AlgoString::string_trim(m_configJson);
        if (!AlgoDirectory::fileExists(m_configJson))
        {
            std::cout << "Config file not exist:" << m_configJson << std::endl;
            throw std::string("Invalid parameter.");
        }

        // check output
        if (!cmd.used('o'))
        {
            std::cout << "No param of output" << std::endl;
            throw std::string("Invalid parameter.");
        }

        AlgoString::string_trim(m_outputPath);
        if (!AlgoDirectory::directoryExists(m_outputPath))
        {
            std::cout << "Output path not exist:" << m_outputPath << std::endl;
            throw std::string("Invalid parameter.");
        }
    }
    catch (const std::string &s)
    {
        std::cout << "Usage: " << argv[0] << '\n'
                  << "[-i|--config] json config file.\n"
                  << "[-o|--output] output path of data.\n";
        std::cout << s;
        return 0;
    }

    // init project for save data
    MyAlgoT1Project _project;
    if (!_project.create(m_outputPath))
    {
        std::cout << "Create project failed!" << std::endl;
        return 0;
    }

    // MyAlgoDraw _draw;

    MyAlgoT1Device _deviceAlgoT1((AlgoT1Project *)&_project);

    // 1.load config
    std::cout << "loading config..." << std::endl;
    if (!_deviceAlgoT1.load(m_configJson))
    {
        std::cout << "Device load config failed!" << std::endl;
        return 0;
    }
    std::cout << "Device load config successed!" << std::endl;

    // 2.start read threads.
    if (!_deviceAlgoT1.start())
    {
        std::cout << "Device start failed!" << std::endl;
        return 0;
    }
    std::cout << "Device start successed!" << std::endl;

    // 3.main thread waitted and show data;
    while (true)
    {
        //_deviceAlgoT1.show(&_draw);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 4.stop read threads
    _project.close();
    _deviceAlgoT1.stop();
    std::cout << "Device stopped!" << std::endl;

    return 0;
}
