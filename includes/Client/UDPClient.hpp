#ifndef _LwM2M_UDP_CLIENT_HPP
#define _LwM2M_UDP_CLIENT_HPP

#include <string>

namespace LwM2M_Client {
std::string receive(bool ip_v6_handler, const std::string &ip_address,
                    unsigned int port);
}
#endif //_LwM2M_UDP_CLIENT_HPP
