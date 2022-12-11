#include "batchGen.h"

BatchGen::BatchGen(int expansionCount) : m_expansionCount(expansionCount)
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
    for (std::vector<ShellParams> list : splitChunks) {
        calculateBatch(list);
    }

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
    shellGenerator.expandCurveNTimes(m_expansionCount);
    shellGenerator.printSurface();
}