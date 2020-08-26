/****************************************************************************/
/**
 *  @file   SocketStandard.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__SOCKET_STANDARD_H_INCLUDE
#define KVS__SOCKET_STANDARD_H_INCLUDE

#include <kvs/Platform>
#if defined( KVS_PLATFORM_WINDOWS )
#include <winsock.h>
#pragma comment(lib,"wsock32.lib")
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#endif // KVS__SOCKET_STANDARD_H_INCLUDE
