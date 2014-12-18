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
	/**SyZmO control messages are used for communication between client and
	server.
	*/
    struct MessageCtrl
	    {
		MessageCtrl(){}

		struct Header
			{
			union
				{
				char ascii[8];
				uint64_t value;
				} magic;           /**<Magic number. Always reads SyZmOmsg.*/
			uint32_t message_type; /**<Message type identifier.*/
			
			/**Number of valid byts in the following message.
			 This value may be zero in case the information is not needed to
			 process the message.*/
			uint32_t message_size; 
			} header;
		
		template<class T>
		MessageCtrl(const T& message)
			{
			header.message_type=T::ID;
			valuesSet(sizeof(T),&message);
			header.message_size=0;
			}

		bool validIs() const;


		
		static const unsigned int HEADER_SIZE=sizeof(Header);
		static const unsigned int DATA_SIZE=128 - HEADER_SIZE;
		
		char data[DATA_SIZE];         /**<Message data.*/

		struct Midi
			{
			static const uint32_t ID=0;
			uint32_t device_id;
			static const unsigned int MESSAGE_COUNT_MAX
				=(DATA_SIZE-sizeof(uint32_t))/sizeof(MessageMidi);
				
			MessageMidi midi[MESSAGE_COUNT_MAX];
			};

		struct IsAliveRequest
			{static const uint32_t ID=1;};

		struct IsAliveResponse
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
			uint32_t device_id;
			uint32_t status;
			static const uint32_t NAME_LENGTH=DATA_SIZE-2*sizeof(uint32_t);
			char name[NAME_LENGTH];
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
			uint32_t device_id;
			uint32_t status;
			static const uint32_t NAME_LENGTH=DATA_SIZE-2*sizeof(uint32_t);
			char name[NAME_LENGTH];
			static const uint32_t STATUS_OK=0;
			static const uint32_t STATUS_BUSY=1;
			static const uint32_t STATUS_INVALID=2;
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
			static const uint32_t STATUS_INVALID=2;
			static const uint32_t NOT_CONNECTED=3;
			};

		struct ConnectionCloseResponsePublic
			{
			static const uint32_t ID=12;
			uint32_t device_id;
			};


		struct ServerHostnameRequest
			{
			static const uint32_t ID=13;
			};

		struct ServerHostnameResponse
			{
			static const uint32_t ID=14;
			static const uint32_t HOSTNAME_LENGTH=DATA_SIZE;
			char hostname[HOSTNAME_LENGTH];
			};

		struct ServerSetupGetRequest
			{
			static const uint32_t ID=15;
			};

		struct ServerSetupGetResponse
			{
			static const uint32_t ID=16;
			ServerSetup setup;
			};

		struct ServerSetupSetRequest
			{
			static const uint32_t ID=17;
			ServerSetup setup;
			};

		struct ServerSetupSetResponse
			{
			static const uint32_t ID=18;
			ServerSetup setup;
			};



		struct ServerStartupResponse
			{static const uint32_t ID=19;};

		struct ServerExitResponse
			{static const uint32_t ID=20;};

		struct ServerShutdownRequest
			{static const uint32_t ID=21;};

		struct ServerRebootRequest
			{static const uint32_t ID=22;};

		struct ServerTestRequest
			{
			static const uint32_t ID=23;
			uint32_t device_id;
			};

		struct ServerTestResponse
			{
			static const uint32_t ID=24;

			uint32_t device_id;
			uint32_t status;
			static const uint32_t NAME_LENGTH=DATA_SIZE-2*sizeof(uint32_t);
		
			char name[NAME_LENGTH];

			static const uint32_t STATUS_OK=0;
			static const uint32_t STATUS_BUSY=1;
			static const uint32_t STATUS_INVALID=2;
			static const uint32_t STATUS_ERROR=(uint32_t)-1;
			};

		struct ServerLogClearRequest
			{static const uint32_t ID=25;};

		struct ServerLogClearResponse
			{static const uint32_t ID=26;};


		private:
			void valuesSet(uint32_t length,const void* vals);
		};
	}

#endif
