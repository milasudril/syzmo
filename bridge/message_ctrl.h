#ifdef __WAND__
target[name[message_ctrl.h] type[include]]
dependency[message_ctrl.o]
#endif

#ifndef SYZMO_MESSAGECTRL_H
#define SYZMO_MESSAGECTRL_H

#include "server_setup.h"
#include "message_midi.h"

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
			{valuesSet(sizeof(T),&message);}

		bool validIs() const;

		union
			{
			char ascii[8];
			uint64_t value;
			} magic;           /**<Magic number. Always reads SyZmOmsg.*/
		uint32_t padding;
		uint32_t message_type; /**<Message type identifier.*/
		char data[48];         /**<Message data.*/

		struct Midi
			{
			static const uint32_t ID=0;
			uint32_t device_id;
			MessageMidi midi;
			};

		struct NoOp
			{static const uint32_t ID=1;};

		struct IsAlive
			{static const uint32_t ID=2;};

		struct DeviceCountRequest
			{static const uint32_t ID=3;};

		struct DeviceCountResponse
			{
			static const uint32_t ID=4;
			uint32_t n_devs;
			};

		struct DeviceNameRequest
			{
			static const uint32_t ID=5;
			uint32_t device_id;
			};

		struct DeviceNameResponse
			{
			static const uint32_t ID=6;
			char name[32];
			uint32_t device_id;
			uint32_t status;
			static const uint32_t STATUS_READY=0;
			static const uint32_t STATUS_BUSY=1;
			static const uint32_t STATUS_INVALID=2;
			};

		struct ConnectionOpenRequest
			{
			static const uint32_t ID=7;
			uint32_t device_id;
			};

		struct ConnectionOpenResponsePrivate
			{
			static const uint32_t ID=8;
			char name[32];
			uint32_t device_id;
			uint32_t status;
			static const uint32_t STATUS_OK=0;
			static const uint32_t STATUS_BUSY=1;
			static const uint32_t STATUS_ERROR=(uint32_t)-1;
			};

		struct ConnectionOpenResponsePublic
			{
			static const uint32_t ID=9;
			uint32_t device_id;
			};

		struct ConnectionCloseRequest
			{
			static const uint32_t ID=10;
			uint32_t device_id;
			};

		struct ConnectionCloseResponsePrivate
			{
			static const uint32_t ID=11;
			uint32_t device_id;
			uint32_t status;
			static const uint32_t STATUS_OK=0;
			static const uint32_t NOT_OWNER=1;
			static const uint32_t NOT_CONNECTED=2;
			};

		struct ConnectionCloseResponsePublic
			{
			static const uint32_t ID=12;
			uint32_t device_id;
			};

		struct ServerSetupRequest
			{
			static const uint32_t ID=13;
			uint32_t flags;
			static const uint32_t PORT_CLIENT_SET=0x1;
			static const uint32_t PORT_SERVER_SET=0x2;
			static const uint32_t TIME_SET=0x4;

			ServerSetup setup;
			};

		struct ServerSetupResponse
			{
			static const uint32_t ID=14;
			ServerSetup setup;
			};

		struct ServerStartupResponse
			{static const uint32_t ID=15;};

		struct ServerExitRequest
			{static const uint32_t ID=16;};

		struct ServerExitResponse
			{static const uint32_t ID=17;};

		struct ServerRestartRequest
			{static const uint32_t ID=18;};

		struct ServerShutdownRequest
			{static const uint32_t ID=19;};

		struct ServerRebootRequest
			{static const uint32_t ID=20;};

		private:
			void valuesSet(uint32_t length,const void* vals);
		};
	}

#endif
