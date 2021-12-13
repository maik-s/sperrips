#include "StructSockPrinter.h"

void StructSockPrinter::print(argument_value value) {
    std::cout << std::endl;

    if (value.str.empty()) {
        printf("  uninitialized struct %s\n", ((std::string)magic_enum::enum_name(value.type)).c_str());
        return;
    }

    if (value.type == AT_STRUCT_SOCKADDR) {
        // unsigned short sa_family;   // address family, AF_xxx
        // char           sa_data[14]; // 14 bytes of protocol address
        struct sockaddr* data = (struct sockaddr*) value.str.c_str();
        printf("  sa_family: %d\n", data->sa_family);
        printf("  sa_data:\n");

        if (data->sa_family == AF_INET) {
            //     struct sockaddr_in {
            //        sa_family_t    sin_family; /* address family: AF_INET */
            //        in_port_t      sin_port;   /* port in network byte order */
            //        struct in_addr sin_addr;   /* internet address */
            //    };

            //    /* Internet address */
            //    struct in_addr {
            //        uint32_t       s_addr;     /* address in network byte order */
            //    };
            struct sockaddr_in* sin = (struct sockaddr_in*) data;
            printf("    sin_family: %d\n", sin->sin_family);
            printf("    sin_port: %d\n", sin->sin_port);
            char str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(sin->sin_addr.s_addr), str, INET_ADDRSTRLEN);
            printf("    sin_addr: %s\n", str);

        } else if (data->sa_family == AF_INET6) {
            //     struct sockaddr_in6 {
            //        sa_family_t     sin6_family;   /* AF_INET6 */
            //        in_port_t       sin6_port;     /* port number */
            //        uint32_t        sin6_flowinfo; /* IPv6 flow information */
            //        struct in6_addr sin6_addr;     /* IPv6 address */
            //        uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
            //    };

            //    struct in6_addr {
            //        unsigned char   s6_addr[16];   /* IPv6 address */
            //    };
            struct sockaddr_in6* sin6 = (struct sockaddr_in6*) data;
            printf("    sin6_family: %d\n", sin6->sin6_family);
            printf("    sin6_port: %d\n", sin6->sin6_port);
            printf("    sin6_port: %d\n", sin6->sin6_flowinfo);
            char str[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &(sin6->sin6_addr.s6_addr), str, INET6_ADDRSTRLEN);
            printf("    sin6_addr: %s\n", str);
            printf("    sin6_scope_id: %d\n", sin6->sin6_scope_id);
        } else if (data->sa_family == AF_UNIX) {
            struct sockaddr_un* un = (struct sockaddr_un*) data;
            printf("    sun_family: %d\n", un->sun_family);
            printf("    sun_path: %s\n", un->sun_path);
        } else {
            Utils::dump(data->sa_data, 14, "");
        }

    } else if (value.type == AT_STRUCT_MSGHDR) {
        // struct msghdr {
        //     void         *msg_name;       /* optional address */
        //     socklen_t     msg_namelen;    /* size of address */
        //     struct iovec *msg_iov;        /* scatter/gather array */
        //     size_t        msg_iovlen;     /* # elements in msg_iov */
        //     void         *msg_control;    /* ancillary data, see below */
        //     size_t        msg_controllen; /* ancillary data buffer len */
        //     int           msg_flags;      /* flags on received message */
        // };
        struct msghdr* data = (struct msghdr*) value.str.c_str();
        printf("  msg_name: %p\n", data->msg_name);
        printf("  msg_namelen: %d\n", data->msg_namelen);
        printf("  msg_iov: %p\n", data->msg_iov);
        printf("  msg_iovlen: %ld\n", data->msg_iovlen);
        printf("  msg_control: %p\n", data->msg_control);
        printf("  msg_controllen: %ld\n", data->msg_controllen);
        printf("  msg_flags: %d\n", data->msg_flags);
    }
}

int StructSockPrinter::getStructLength(argument_type type) {
    if (type == AT_STRUCT_SOCKADDR) {
        return sizeof(sockaddr);
    } else if (type == AT_STRUCT_MSGHDR) {
        return sizeof(msghdr);
    } else {
        return -1;
    }
}

std::string StructSockPrinter::updateStructAdresses(argument_type type, std::string recorded, std::string replayed, ASLRMapper* pointerMapper) {
    if (recorded.empty() || replayed.empty()) {
        return replayed;
    }

    assert(recorded != replayed);
    std::cout << "[i] Fixing pointers in struct" << std::endl;
    struct msghdr rec = {};
    struct msghdr rep = {};

    memcpy(&rec, recorded.c_str(), sizeof(struct msghdr));
    memcpy(&rep, replayed.c_str(), sizeof(struct msghdr));
    // Utils::dump(replayed.c_str(), sizeof(struct msghdr), " msghdr:");

    pointerMapper->map(rec.msg_name, rep.msg_name);
    pointerMapper->map(rec.msg_iov, rep.msg_iov);
    pointerMapper->map(rec.msg_control, rep.msg_control);

    rec.msg_name = rep.msg_name;
    rec.msg_iov = rep.msg_iov;
    rec.msg_control = rep.msg_control;

    std::string res = std::string((char*) &rec, sizeof(struct msghdr));

    assert(res.size() == recorded.size() && res.size() == sizeof(struct msghdr));
    return res;
}