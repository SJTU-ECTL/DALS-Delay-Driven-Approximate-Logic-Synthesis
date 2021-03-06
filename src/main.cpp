/**
 * @file main.cpp
 * @brief
 * @author Nathan Zhou
 * @date 2019-01-13
 * @bug No known bugs.
 */

#include <iostream>
#include <iomanip>
#include <bitset>
#include <boost/filesystem.hpp>
#include <abc_plus.h>
#include <sta.h>
#include <dals.h>
#include <playground.h>

using namespace boost::filesystem;
using namespace abc_plus;

void Test();

void Execute();

void PreproBenchtoAigBlif(const path &bench_dir, const path &blif_dir, const std::vector<std::string> &files);

int main(int argc, char *argv[]) {
    Test();
    Execute();
    return 0;
}

void Test() {
    auto playground = Playground::GetPlayground();
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    std::cout << "> Max Flow Min Cut" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    playground->MaxFlowMinCut();
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    std::cout << "> Approximate Substitution" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    playground->ApproximateSubstitution(false);
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    std::cout << "> Static Timing Analysis" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    playground->StaticTimingAnalysis();
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    std::cout << "> Critical Graph" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    playground->CriticalGraph();
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    std::cout << "> Critical Error Network" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    playground->CriticalErrorNetwork();
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    std::cout << "> Test Finished" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
}

void Execute() {
    path project_source_dir(PROJECT_SOURCE_DIR);
    path out_dir = project_source_dir / "out";
    path bench_dir = project_source_dir / "benchmark" / "bench";
    path blif_dir = project_source_dir / "benchmark" / "blif";
    std::vector<std::string> iscas_85 = {"c17", "c432", "c499", "c880", "c1355", "c1908", "c2670", "c3540", "c5315", "c6288", "c7552"};

    std::string blif_name = "c17.blif";
    path blif_file = blif_dir / blif_name;
//    PreproBenchtoAigBlif(bench_dir, blif_dir, iscas_85);
    auto framework = Framework::GetFramework();
    framework->ReadBlif(blif_file.string());
    auto ntk = framework->GetNtk();
    auto dals = DALS::GetDALS();
    dals->SetTargetNtk(ntk);
    dals->SetSim64Cycles(10000);
    dals->Run(0.15);

    path approx_blif_file = out_dir / blif_name;
    auto approx_ntk = dals->GetApproxNtk();
    NtkWriteBlif(approx_ntk, approx_blif_file.string());
}

void PreproBenchtoAigBlif(const path &bench_dir, const path &blif_dir, const std::vector<std::string> &files) {
    auto framework = Framework::GetFramework();
    for (const auto &file : files) {
        path bench_file = bench_dir / file;
        bench_file.replace_extension("bench");
        path blif_file = blif_dir / file;
        blif_file.replace_extension("blif");

        framework->ReadBench(bench_file.string());
        framework->CmdExec("strash");
        framework->WriteBlif(blif_file.string());
    }
}
