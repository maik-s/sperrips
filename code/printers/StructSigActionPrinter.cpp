#include "StructSigActionPrinter.h"

void StructSigActionPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    struct sigaction* data = (struct sigaction*) value.str.c_str();

    printf("  sa_handler: %p\n", data->sa_handler);

    printf("  sa_sigaction: %p\n", data->sa_sigaction);

    printf("  sa_mask: %lu\n", *data->sa_mask.__val);

    printf("  sa_flags: %d\n", data->sa_flags);

    printf("  sa_restorer: %p\n", data->sa_restorer);
}

int StructSigActionPrinter::getStructLength(argument_type type) {
    return sizeof(struct sigaction);
}

std::string StructSigActionPrinter::updateStructAdresses(argument_type type, std::string recorded, std::string replayed, ASLRMapper* pointerMapper) {
    if (recorded.empty() || replayed.empty()) {
        // assert (false); // dont yet wether this could appear ! needs investigation
        return replayed;
    }

    struct sigaction rec = {};

    struct sigaction rep = {};

    memcpy(&rec, recorded.c_str(), sizeof(struct sigaction));

    memcpy(&rep, replayed.c_str(), sizeof(struct sigaction));

    pointerMapper->map((void*)rec.sa_handler, (void*)rep.sa_handler);

    pointerMapper->map((void*)rec.sa_sigaction, (void*)rep.sa_sigaction);

    pointerMapper->map((void*)rec.sa_restorer, (void*)rep.sa_restorer);

    rec.sa_handler = rep.sa_handler;

    rec.sa_sigaction = rep.sa_sigaction;

    rec.sa_restorer = rep.sa_restorer;

    std::string res = std::string((char*) &rec, sizeof(struct sigaction));

    // std::cout << recorded.size() << " vs. " << res.size() << std::endl;
    assert(res.size() == recorded.size());

    return res;
}