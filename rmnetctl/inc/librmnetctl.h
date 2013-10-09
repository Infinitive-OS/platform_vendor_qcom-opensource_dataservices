/******************************************************************************

			  L I B R M N E T C T L . H

Copyright (c) 2013, The Linux Foundation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above
	  copyright notice, this list of conditions and the following
	  disclaimer in the documentation and/or other materials provided
	  with the distribution.
	* Neither the name of The Linux Foundation nor the names of its
	  contributors may be used to endorse or promote products derived
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

/*!
*  @file    librmnetctl.h
*  @brief   rmnet control API's header file
*/

#ifndef LIBRMNETCTL_H
#define LIBRMNETCTL_H

/* RMNET API succeeded */
#define RMNETCTL_SUCCESS 0
/* RMNET API encountered an error while executing within the library. Check the
* error code in this case */
#define RMNETCTL_LIB_ERR 1
/* RMNET API encountered an error while executing in the kernel. Check the
* error code in this case */
#define RMNETCTL_KERNEL_ERR 2
/* RMNET API encountered an error because of invalid arguments*/
#define RMNETCTL_INVALID_ARG 3

/* Flag to associate a network device*/
#define RMNETCTL_DEVICE_ASSOCIATE 1
/* Flag to unassociate a network device*/
#define RMNETCTL_DEVICE_UNASSOCIATE 0
/* Flag to create a new virtual network device*/
#define RMNETCTL_NEW_VND 1
/* Flag to free a new virtual network device*/
#define RMNETCTL_FREE_VND 0

enum rmnetctl_error_codes_e {
	/* API succeeded. This should always be the first element. */
	RMNETCTL_API_SUCCESS,
	/* API failed because not enough memory to create buffer to send
	 * message */
	RMNETCTL_API_ERR_REQUEST_INVALID,
	/* API failed because not enough memory to create buffer for the
	 *  response message */
	RMNETCTL_API_ERR_RESPONSE_INVALID,
	/* API failed because could not send the message to kernel */
	RMNETCTL_API_ERR_MESSAGE_SEND,
	/* API failed because could not receive message from the kernel */
	RMNETCTL_API_ERR_MESSAGE_RECEIVE,
	/* Invalid process id. So return an error. */
	RMNETCTL_INIT_ERR_PROCESS_ID,
	/* Invalid socket descriptor id. So return an error. */
	RMNETCTL_INIT_ERR_NETLINK_FD,
	/* Could not bind the socket to the Netlink file descriptor */
	RMNETCTL_INIT_ERR_BIND,
	/* Invalid user id. Only root has access to this function. (NA) */
	RMNETCTL_INIT_ERR_INVALID_USER,
	/* API failed because the RmNet handle for the transaction was NULL */
	RMNETCTL_API_ERR_HNDL_INVALID,
	/* API failed because the request buffer for the transaction was NULL */
	RMNETCTL_API_ERR_REQUEST_NULL,
	/* API failed because the response buffer for the transaction was NULL*/
	RMNETCTL_API_ERR_RESPONSE_NULL,
	/* API failed because the request and response type do not match*/
	RMNETCTL_API_ERR_MESSAGE_TYPE,
	/* API failed because the return type is invalid */
	RMNETCTL_API_ERR_RETURN_TYPE,
	/* API failed because the string was truncated */
	RMNETCTL_API_ERR_STRING_TRUNCATION,
	/* This should always be the last element */
	RMNETCTL_API_ERR_ENUM_LENGTH
};

#define RMNETCTL_ERR_MSG_SIZE 100

/*!
* @brief Contains a list of error message from API
*/
char rmnetctl_error_code_text
[RMNETCTL_API_ERR_ENUM_LENGTH][RMNETCTL_ERR_MSG_SIZE] = {
	"ERROR: API succeeded\n",
	"ERROR: Unable to allocate the buffer to send message\n",
	"ERROR: Unable to allocate the buffer to receive message\n",
	"ERROR: Could not send the message to kernel\n",
	"ERROR: Unable to receive message from the kernel\n",
	"ERROR: Invalid process id\n",
	"ERROR: Invalid socket descriptor id\n",
	"ERROR: Could not bind to netlink socket\n",
	"ERROR: Only root can access this API\n",
	"ERROR: RmNet handle for the transaction was NULL\n",
	"ERROR: Request buffer for the transaction was NULL\n",
	"ERROR: Response buffer for the transaction was NULL\n",
	"ERROR: Request and response type do not match\n",
	"ERROR: Return type is invalid\n",
	"ERROR: String was truncated\n"
};

/*===========================================================================
			 DEFINITIONS AND DECLARATIONS
===========================================================================*/
typedef struct rmnetctl_hndl_s rmnetctl_hndl_t;

/*!
* @brief Public API to initialize the RMNET control driver
* @details Allocates memory for the RmNet handle. Creates and binds to a   and
* netlink socket if successful
* @param **rmnetctl_hndl_t_val RmNet handle to be initialized
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnetctl_init(rmnetctl_hndl_t **hndl, uint16_t *error_code);

/*!
* @brief Public API to clean up the RmNeT control handle
* @details Close the socket and free the RmNet handle
* @param *rmnetctl_hndl_t_val RmNet handle to be initialized
* @return void
*/
void rmnetctl_cleanup(rmnetctl_hndl_t *hndl);

/*!
* @brief Public API to register/unregister a RMNET driver on a particular device
* @details Message type is RMNET_NETLINK_ASSOCIATE_NETWORK_DEVICE or
* RMNET_NETLINK_UNASSOCIATE_NETWORK_DEVICE based on the flag for assoc_dev
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param dev_name Device on which to register the RmNet driver
* @param error_code Status code of this operation
* @param assoc_dev registers the device if RMNETCTL_DEVICE_ASSOCIATE or
* unregisters the device if RMNETCTL_DEVICE_UNASSOCIATE
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_associate_network_device(rmnetctl_hndl_t *hndl,
				   const char *dev_name,
				   uint16_t *error_code,
				   uint8_t assoc_dev);

/*!
* @brief Public API to get if a RMNET driver is registered on a particular
* device
* @details Message type is RMNET_NETLINK_GET_NETWORK_DEVICE_ASSOCIATED.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param dev_name Device on which to check if the RmNet driver is registered
* @param register_status 1 if RmNet data driver is registered on a particular
* device, 0 if not
* @param error_code Status code of this operation
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_get_network_device_associated(rmnetctl_hndl_t *hndl,
					const char *dev_name,
					int *register_status,
					uint16_t *error_code);

/*!
* @brief Public API to set the egress data format for a particular link.
* @details Message type is RMNET_NETLINK_SET_LINK_EGRESS_DATA_FORMAT.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param egress_flags Egress flags to be set on the device
* @param agg_size Max size of aggregated packets
* @param agg_count Number of packets to be aggregated
* @param dev_name Device on which to set the egress data format
* @param error_code Status code of this operation returned from the kernel
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_set_link_egress_data_format(rmnetctl_hndl_t *hndl,
				      uint32_t egress_flags,
				      uint16_t agg_size,
				      uint16_t agg_count,
				      const char *dev_name,
				      uint16_t *error_code);

/*!
* @brief Public API to get the egress data format for a particular link.
* @details Message type is RMNET_NETLINK_GET_LINK_EGRESS_DATA_FORMAT.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param dev_name Device on which to get the egress data format
* @param egress_flags Egress flags from the device
* @param agg_count Number of packets to be aggregated
* @param error_code Status code of this operation returned from the kernel
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_get_link_egress_data_format(rmnetctl_hndl_t *hndl,
				      const char *dev_name,
				      uint32_t *egress_flags,
				      uint16_t *agg_size,
				      uint16_t *agg_count,
				      uint16_t *error_code);

/*!
* @brief Public API to set the ingress data format for a particular link.
* @details Message type is RMNET_NETLINK_SET_LINK_INGRESS_DATA_FORMAT.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param ingress_flags Ingress flags from the device
* @param dev_name Device on which to set the ingress data format
* @param error_code Status code of this operation returned from the kernel
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_set_link_ingress_data_format(rmnetctl_hndl_t *hndl,
				       uint32_t ingress_flags,
				       const char *dev_name,
				       uint16_t *error_code);

/*!
* @brief Public API to get the ingress data format for a particular link.
* @details Message type is RMNET_NETLINK_GET_LINK_INGRESS_DATA_FORMAT.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param dev_name Device on which to get the ingress data format
* @param ingress_flags Ingress flags from the device
* @param error_code Status code of this operation returned from the kernel
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_get_link_ingress_data_format(rmnetctl_hndl_t *hndl,
				       const char *dev_name,
				       uint32_t *ingress_flags,
				       uint16_t *error_code);

/*!
* @brief Public API to set the logical endpoint configuration for a
* particular link.
* @details Message type is RMNET_NETLINK_SET_LOGICAL_EP_CONFIG.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param logical_ep_id Logical end point id on which the configuration is to be
* set
* @param rmnet_mode RmNet mode to be set on the device
* @param dev_name Device on which to set the logical end point configuration
* @param egress_dev_name Egress Device if operating in bridge mode
* @param error_code Status code of this operation returned from the kernel
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_set_logical_ep_config(rmnetctl_hndl_t *hndl,
				int32_t ep_id,
				uint8_t operating_mode,
				const char *dev_name,
				const char *next_dev,
				uint16_t *error_code);

/*!
* @brief Public API to get the logical endpoint configuration for a
* particular link.
* @details Message type is RMNET_NETLINK_GET_LOGICAL_EP_CONFIG.
* @param *rmnetctl_hndl_t_val RmNet handle for the Netlink message
* @param logical_ep_id Logical end point id from which to get the configuration
* @param dev_name Device on which to get the logical end point configuration
* @param rmnet_mode RmNet mode from the device
* @param egress_dev_name Egress Device if operating in bridge mode
* @param error_code Status code of this operation returned from the kernel
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_get_logical_ep_config(rmnetctl_hndl_t *hndl,
				int32_t ep_id,
				const char *dev_name,
				uint8_t *operating_mode,
				char **next_dev,
				uint16_t *error_code);

/*!
* @brief Public API to create a new virtual device node
* @details Message type is RMNET_NETLINK_NEW_VND or
* RMNETCTL_FREE_VND based on the flag for new_vnd
* @param hndl RmNet handle for the Netlink message
* @param id Node number to create the virtual network device node
* @param error_code Status code of this operation returned from the kernel
* @param new_vnd creates a new virtual network device if  RMNETCTL_NEW_VND or
* frees the device if RMNETCTL_FREE_VND
* @return RMNETCTL_SUCCESS if successful
* @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
* @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
* Check error_code
* @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
*/
int rmnet_new_vnd(rmnetctl_hndl_t *hndl,
		  uint32_t id,
		  uint16_t *error_code,
		  uint8_t new_vnd);

/*!
 * @brief Public API to create a new virtual device node with a custom prefix
 * @details Message type is RMNET_NETLINK_NEW_VND or
 * RMNETCTL_FREE_VND based on the flag for new_vnd
 * @param hndl RmNet handle for the Netlink message
 * @param id Node number to create the virtual network device node
 * @param error_code Status code of this operation returned from the kernel
 * @param new_vnd creates a new virtual network device if  RMNETCTL_NEW_VND or
 * frees the device if RMNETCTL_FREE_VND
 * @param prefix Prefix to be used when naming the network interface
 * @return RMNETCTL_SUCCESS if successful
 * @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
 * @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
 * Check error_code
 * @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
 */
int rmnet_new_vnd_prefix(rmnetctl_hndl_t *hndl,
			 uint32_t id,
			 uint16_t *error_code,
			 uint8_t new_vnd,
			 const char *prefix);
/*!
 * @brief API to get the ASCII name of a virtual network device from its ID
 * @param hndl RmNet handle for the Netlink message
 * @param id Node number to create the virtual network device node
 * @param error_code Status code of this operation returned from the kernel
 * @param buf Buffer to store ASCII representation of device name
 * @param buflen Length of the buffer
 * @param prefix Prefix to be used when naming the network interface
 * @return RMNETCTL_SUCCESS if successful
 * @return RMNETCTL_LIB_ERR if there was a library error. Check error_code
 * @return RMNETCTL_KERNEL_ERR if there was an error in the kernel.
 * Check error_code
 * @return RMNETCTL_INVALID_ARG if invalid arguments were passed to the API
 */

int rmnet_get_vnd_name(rmnetctl_hndl_t *hndl,
                      uint32_t id,
                      uint16_t *error_code,
                      char *buf,
                      uint32_t buflen);

#endif /* not defined LIBRMNETCTL_H */

