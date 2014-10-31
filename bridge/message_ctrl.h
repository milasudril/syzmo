#ifdef __WAND__
target[name[message_ctrl.h] type[include]]
dependency[message_ctrl.o]
#endif

#ifndef SYZMO_MESSAGECTRL_H
#define SYZMO_MESSAGECTRL_H

#include "inttypes.h"

namespace SyZmO
	{
	/**SyZmO control messages are used for non-MIDI communication between client and
	server.
	*/
    struct MessageCtrl
	    {
		MessageCtrl(){}

		template<class T>
		MessageCtrl(const T& message):
			message_type(T::ID)
			{
			valuesSet(sizeof(T),&message);
			}

		union
			{
			char ascii[8];
			uint64_t value;
			} magic;           /**<Magic number. Always reads SyZmOmsg.*/
		int32_t timestamp;     /**<POSIX time when the message was created.*/
		uint32_t message_type; /**<Message type identifier.*/
		char data[48];         /**<Message data.*/

		struct NoOp
			{
			static const uint32_t ID=0;
			};

		struct IsAlive
			{
			static const uint32_t ID=1;
			};

		struct ServerStartup
			{
			static const uint32_t ID=2;
			};

		struct ServerShutdown
			{
			static const uint32_t ID=3;
			};

		struct DeviceCountRequest
			{
			static const uint32_t ID=4;
			};

		struct DeviceCountResponse
			{
			static const uint32_t ID=5;
			uint32_t n_devs;
			};

		struct DeviceNameRequest
			{
			static const uint32_t ID=6;
			uint32_t id;
			};

		struct DeviceNameResponse
			{
			static const uint32_t ID=7;
			char name[32];
			};

		private:
			void valuesSet(uint32_t length,const void* vals);
		};
	}

#endif
