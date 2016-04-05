#ifndef __ESP_H_DEFINED__
#define __ESP_H_DEFINED__

// message header and footer bytes
#define ESP_HEADER1 127
#define ESP_HEADER2 83
#define ESP_FOOTER 5

// message IDs
#define MSG_GPS 0
#define MSG_CONTROL 1
#define MSG_COMMAND 2
#define MSG_SET_PID 3

// message format: ESP_HEADER1 ESP_HEADER2 MESSAGE_ID <data> ESPFOOTER

/** @file */

/** Pack a signed 32 bit integer into an unsigned integer array as 4 bytes. LEAST significant bit comes FIRST
  * @param[in] uint8_t* buffy array to put values in
  * @param[in] int32_t var value to put in array
  */
void pack_int32(uint8_t* buffy,int32_t var){
	buffy[0] = var & 255;
	buffy[1] = (var>>8) & 255;
	buffy[2] = (var>>16) & 255;
	buffy[3] = (var>>24) & 255;
};

/** Read a signed 32 bit integer from an unsigned integer array as 4 bytes. LEAST significant bit comes FIRST
  * @param[in] uint8_t* buffy array to put values in
  * @param[out] int32_t var value to read from array
  */
void unpack_int32(uint8_t* buffy, int32_t* var){
	*var = buffy[0] + (buffy[1]<<8) + (buffy[2]<<16) + (buffy[3]<<24);
}

/** @brief Pack a GPS message
 *  @param[in] uint8_t*msg message buffer to pack the message into
 *	@param[in] int32_t lon Londgitude in (degrees x 10^-7) mapped from -180 to 180 deg
 * 	@param[in] int32_t lat Latitude in (degrees x 10^-7) 
 *	@param[in] float time gps time, if available
 * 	\return {int8_t len: length of message}
 */
inline int8_t esp_pack_gps(uint8_t*msg,int32_t lon, int32_t lat, float time){
	int8_t msg_counter = 0;
	msg[msg_counter] = ESP_HEADER1;
	msg_counter++;
	msg[msg_counter] = ESP_HEADER2;
	msg_counter++;
	// message ID
	msg[msg_counter] = MSG_GPS;
	msg_counter++;
	// pack longitude
	pack_int32(&msg[msg_counter], lon);
	msg_counter += 4;
	// pack latitude
	pack_int32(&msg[msg_counter], lat);
	msg_counter += 4;
	// time
	memcpy(&msg[msg_counter],&time,4);
	msg_counter += 4;
	//end byte
	msg[msg_counter] = ESP_FOOTER;
	msg_counter++;
	return msg_counter;
};
/** Unpack a GPS message into longitude, latitude, and time
 *
 */
inline int8_t esp_unpack_gps(uint8_t*msg,int32_t* lon, int32_t* lat, float* time){
	//check the header bytes
	if (msg[0] != ESP_HEADER1 | msg[1] != ESP_HEADER2)
		return -1;
	//pass over the message ID byte
	int8_t msg_counter=3;
	*lon = msg[msg_counter] + (msg[msg_counter+1]<<8) + (msg[msg_counter+2]<<16) + (msg[msg_counter+3]<<24);
	msg_counter+=4;
	*lat = msg[msg_counter] + (msg[msg_counter+1]<<8) + (msg[msg_counter+2]<<16) + (msg[msg_counter+3]<<24);
	msg_counter+=4;
	memcpy(time,&msg[msg_counter],4);
	msg_counter = msg_counter + 1;
	//check footer bytes
	if (msg[msg_counter] != ESP_FOOTER)
		return -1;
	return msg_counter;
};

/** @brief Pack a low-level control message
 *  @param[in] uint8_t*msg message buffer to pack the message into
 *	@param[in] float rudd rudder command mapped from -1 to 1
 * 	@param[in] float thro throttle command mapped from 0 to 1
 * 	\return {int8_t len: length of message}
 */
inline int8_t esp_pack_control(uint8_t*msg,float rudd, float thro){
	//header bytes
	int8_t msg_counter = 0;
	msg[msg_counter] = ESP_HEADER1;
	msg_counter++;
	msg[msg_counter] = ESP_HEADER2;
	msg_counter++;
	// message ID
	msg[msg_counter] = MSG_CONTROL;
	msg_counter++;
	// pack rudder
	memcpy(&msg[msg_counter],&rudd,4);
	msg_counter += 4;
	// pack throttle
	memcpy(&msg[msg_counter],&thro,4);
	msg_counter += 4;
	//end byte
	msg[msg_counter] = ESP_FOOTER;
	msg_counter++;
	return msg_counter;
};

inline int8_t esp_unpack_control(uint8_t*msg,float* rudd, float* thro){
	//check the header bytes
	if (msg[0] != ESP_HEADER1 | msg[1] != ESP_HEADER2)
		return -1;
	// parse data
	int8_t msg_counter = 3;
	//rudder
	memcpy(rudd,&msg[msg_counter],4);
	msg_counter+=4;
	//throttle
	memcpy(thro,&msg[msg_counter],4);
	msg_counter+=4;
	//check footer bytes
	if (msg[msg_counter] != ESP_FOOTER)
		return -1;
	return msg_counter;
}

/** @brief Pack a higher-level command with a desired direction and speed
 *  @param[in] uint8_t*msg message buffer to pack the message into
 * 	@param[in] float hdg; desired heading in radians
 *	@param[in] float speed; speed mapped from 0 to 1; equivalent to throttle mapping
 * 	\return {int8_t len: length of message}
 */
inline int8_t esp_pack_command(uint8_t*msg,float hdg, float speed){
	//header bytes
	int8_t msg_counter = 0;
	msg[msg_counter] = ESP_HEADER1;
	msg_counter++;
	msg[msg_counter] = ESP_HEADER2;
	msg_counter++;
	// message ID
	msg[msg_counter] = MSG_COMMAND;
	msg_counter++;
	// pack heading
	memcpy(&msg[msg_counter],&hdg,4);
	msg_counter += 4;
	// pack speed
	memcpy(&msg[msg_counter],&speed,4);
	msg_counter += 4;
	//end byte
	msg[msg_counter] = ESP_FOOTER;
	msg_counter++;
	return msg_counter;
};

inline int8_t esp_unpack_command(uint8_t*msg,float* hdg, float* speed){
	//check the header bytes
	if (msg[0] != ESP_HEADER1 | msg[1] != ESP_HEADER2)
		return -1;
	// parse data
	int8_t msg_counter = 3;
	//hdg
	memcpy(hdg,&msg[msg_counter],4);
	msg_counter+=4;
	//speed
	memcpy(speed,&msg[msg_counter],4);
	msg_counter+=4;
	//check footer bytes
	if (msg[msg_counter] != ESP_FOOTER)
		return -1;
	return msg_counter;
}

/** @brief Pack a PID message to set PID gains
 *  @param[in] uint8_t*msg message buffer to pack the message into
 *  @param[in] uint8_t ch channel to set gains; 0 is steering, 1 is throttle
 * 	@param[in] float KP proportional gain
 * 	@param[in] float KI integral gain
 * 	@param[in] float KD derivative gain
 * 	\return {int8_t len: length of message}
 */
inline int8_t esp_pack_set_pid(uint8_t*msg,uint8_t ch,float KP, float KI, float KD){
	//header bytes
	int8_t msg_counter = 0;
	msg[msg_counter] = ESP_HEADER1;
	msg_counter++;
	msg[msg_counter] = ESP_HEADER2;
	msg_counter++;
	// message ID
	msg[msg_counter] = MSG_SET_PID;
	msg_counter++;
	// pack channel
	msg[msg_counter] = ch;
	msg_counter++;
	// pack proportional gain
	memcpy(&msg[msg_counter],&KP,4);
	msg_counter += 4;
	// pack integral gain
	memcpy(&msg[msg_counter],&KI,4);
	msg_counter += 4;
	// pack derivative gain
	memcpy(&msg[msg_counter],&KD,4);
	msg_counter += 4;
	//end byte
	msg[msg_counter] = ESP_FOOTER;
	msg_counter++;
	return msg_counter;
};

inline int8_t esp_unpack_set_pid(uint8_t*msg,uint8_t* ch,float* KP, float* KI, float* KD){
	//check the header bytes
	if (msg[0] != ESP_HEADER1 | msg[1] != ESP_HEADER2)
		return -1;
	// parse data
	int8_t msg_counter = 3;
	// channel
	memcpy(ch,&msg[msg_counter],1);
	msg_counter+=1;
	//proportional gain
	memcpy(KP,&msg[msg_counter],4);
	msg_counter+=4;
	//integral gain
	memcpy(KI,&msg[msg_counter],4);
	msg_counter+=4;
	//derivative gain
	memcpy(KD,&msg[msg_counter],4);
	msg_counter+=4;
	//check footer bytes
	if (msg[msg_counter] != ESP_FOOTER)
		return -1;
	return msg_counter;
}

#endif