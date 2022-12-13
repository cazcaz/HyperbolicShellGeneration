#include "batchGen.h"

BatchGen::BatchGen()
{
    m_threadCount = std::thread::hardware_concurrency();
}

BatchGen::~BatchGen()
{
}

void BatchGen::calculateAll(std::vector<ShellParams> parameterList)
{   
    if (m_threadCount == 0) {
        std::cout << "Failed, no threads found." << std::endl;
        return;
    }
    int chunkCount = parameterList.size()/m_threadCount + 1;
    std::vector<std::vector<ShellParams>> splitChunks(chunkCount);
    int lengthCount = 0;
    int chunkIndex = 0;
    for (ShellParams parameters : parameterList) {
        splitChunks[chunkIndex].push_back(parameters);
        lengthCount++;
        if (lengthCount >= m_threadCount) {
            lengthCount = 0;
            chunkIndex++;
        }
    }
    
    int barWidth = 70;
    float progress = 0.0;
    float progressJump = 1.0/parameterList.size();
    for (std::vector<ShellParams> list : splitChunks) {
        std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
        calculateBatch(list);
        progress += list.size()*progressJump; // for demonstration only
    }
    std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
    std::cout << std::endl;    
}

void BatchGen::calculateBatch(std::vector<ShellParams> parameterList)
{
    int threadsNeeded = parameterList.size();
    std::vector<std::thread> threads(threadsNeeded);
    for (int i = 0; i < threadsNeeded; i++) {
        threads[i] = std::thread(&BatchGen::OnThreadShellGen, this,parameterList[i]);
    }
    for (auto& th : threads) {
        th.join();
    }
}

void BatchGen::OnThreadShellGen(ShellParams parameters){
    ShellGen shellGenerator(parameters);
    shellGenerator.setInitCurve();
    shellGenerator.expandCurveNTimes();
    shellGenerator.printSurface();
}