#ifndef __COMMPARSER_H_DEFINED__
#define __COMMPARSER_H_DEFINED__

#include <stdint.h>
#include "emilyStatus.h"
#include "ESP.h"

/** After this many milliseconds without receiving a message, set comm mode to WARNING */
#define TIMEOUT_WARNING_MILLIS 1000
/** After this many milliseconds without receiving a message, set comm mode to LOST */
#define TIMEOUT_LOST_MILLIS 10000
/** Target stream rate for GPS messages in millliseconds */
#define STREAM_RATE_GPS 1000

class commParser
{
	public:
		/** Class constructor, initializes counters to zero
		  * @param[in] st: a pointer to a status class object
		  */
		commParser(emilyStatus*st);
		/** Feed new bytes received to the parser
		  *
		  */
		void newBytes(uint8_t*bytes,int len,uint32_t millis);
		uint32_t get_bad_packets();
		uint32_t get_bad_checksums();
		uint8_t msg[256]; /*!< Message buffer used with ESP */ // should be private
		/** Perform miscellaneous tasks
		 *
		 *  @detail Check the time since last received message and update the status
		 *					Send periodic messages
		 *	@param[in] Current clock time in milliseconds
		 */
		void misc_tasks(uint32_t millis);
	private:
		/** Generic message that is called whenever we receive a the last byte of a message.
		  *
		  */
		void handleMsg();
		emilyStatus*status; /*!< system status class */
		uint32_t bad_checksums;/*!< counter for number of bad checksums received */
		uint32_t bad_packets;/*!< counter for rejected packets. If this increments, we probably have a problem in the parser. */
		uint32_t last_message_millis;/*!< last message receive time in millis */
};

#endif
