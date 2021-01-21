#include <algorithm>
#include <memory>
#include <iomanip>
#include <proc/readproc.h>

#include "challenge.h"

namespace proc {

struct proc_info {
    int id;
    std::string name;
    std::string usr;
    unsigned long mems;

    void print(std::ostream& out) const {
        out << std::left << std::setw(25) << std::setfill(' ')
            << name;
        out << std::left << std::setw(8) << std::setfill(' ')
            << id;
        out << std::left << std::setw(15) << std::setfill(' ')
            << usr;
        out << std::right << std::setw(8) << std::setfill(' ')
            << mems;
        out << std::endl;
    }

    proc_info(int id, char const* name, char const* usr, unsigned long mems)
        : id{id}, name{name}, usr{usr}, mems{mems}
    {
    }
};

std::vector<proc_info> read_procinfo() {
    std::vector<proc_info> ret;
    auto proc = std::unique_ptr<PROCTAB, void(&)(PROCTAB*)>(
            openproc(PROC_FILLARG | PROC_FILLSTAT | PROC_FILLUSR | PROC_FILLSTATUS), closeproc);
    if (!proc) {
        std::cerr << "openproc failed." << std::endl;
    } else {
        while (std::unique_ptr<proc_t, void(&)(proc_t*)> proc_info{ readproc(proc.get(), nullptr), freeproc})
            ret.emplace_back(proc_info->tid, proc_info->cmd, proc_info->euser, proc_info->vm_size);
    }
    return ret;
}

} // end of namespace proc

void test0() {
    auto procs = proc::read_procinfo();
    std::ranges::sort(procs, {}, &proc::proc_info::name); // compare のデフォルトは std::ranges::less
    for (auto const& proc : procs) {
        proc.print(std::cout);
    }
}

int main(int, char**) {
    test0();
    return 0;
}
