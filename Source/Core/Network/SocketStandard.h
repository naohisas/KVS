/****************************************************************************/
/**
 *  @file SocketStandard.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SocketStandard.h 631 2010-10-10 02:15:35Z naohisa.sakamoto $
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
