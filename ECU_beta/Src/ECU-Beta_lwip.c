

/* Includes ------------------------------------------------------------------*/
#include <ECU_Beta_lwip.h>
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/udp.h"
#include  <math.h>
#include  <stdio.h>

/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/


uint8_t data[100];

struct udp_pcb* elmo_R;
struct udp_pcb* elmo_L;
/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];



void MX_LWIP_Init(void)
{
  /* IP addresses initialization */
  IP_ADDRESS[0] 		= 192;
  IP_ADDRESS[1] 		= 168;
  IP_ADDRESS[2] 		= 1;
  IP_ADDRESS[3] 		= 51;
  NETMASK_ADDRESS[0] 	= 255;
  NETMASK_ADDRESS[1] 	= 255;
  NETMASK_ADDRESS[2] 	= 255;
  NETMASK_ADDRESS[3]	= 0;
  GATEWAY_ADDRESS[0] 	= 0;
  GATEWAY_ADDRESS[1] 	= 0;
  GATEWAY_ADDRESS[2] 	= 0;
  GATEWAY_ADDRESS[3] 	= 0;

  /* Initilialize the LwIP stack without RTOS */
  lwip_init();

  /* IP addresses initialization without DHCP (IPv4) */
  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4/IPv6) without RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

}


void udp_receive_callback_R(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
      /* allocate pbuf from pool*/
     //p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
      if (p != NULL)
      {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, strlen((char*)data));

        /* send udp data */
        udp_send(upcb, p);

        /* free pbuf */
        pbuf_free(p);
      }
}

void udp_receive_callback_L(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{

	/* allocate pbuf from pool*/
     //p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);

      if (p != NULL)
      {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, strlen((char*)data));

        /* send udp data */
        udp_send(upcb, p);

        /* free pbuf */
        pbuf_free(p);
      }
}

err_t Create_UDP_Sockets(){

	ip4_addr_t destIPAddr1;
	ip4_addr_t destIPAddr2;

	err_t err 	= ERR_OK;
	elmo_R 		= udp_new();
	elmo_L 		= udp_new();

    if (elmo_R == NULL || elmo_L == NULL)
    {
        return ERR_MEM;
    }

    // Load the static IP of the destination address
    IP4_ADDR(&destIPAddr1,192,168,1,49);
    IP4_ADDR(&destIPAddr2,192,168,1,48);
 //   IP4_ADDR(&destIPAddr1,169,254,104,249);
 //   IP4_ADDR(&destIPAddr2,169,254,104,249);

    // Should bind to the local ip and port
    err = udp_bind(elmo_R,IP4_ADDR_ANY,5002);

    if (err != ERR_OK){
        return err;
    }

    // Connect to the other port
    err = udp_connect(elmo_R,&destIPAddr1,5001);

    if (err != ERR_OK){
        return err;
    }

    // Set the receive function
    udp_recv(elmo_R,udp_receive_callback_R,NULL);

    // Should bind to the local ip and port
    err = udp_bind(elmo_L,IP4_ADDR_ANY,5004);

    if (err != ERR_OK){
        return err;
    }

    // Connect to the other port
    err = udp_connect(elmo_L,&destIPAddr2,5001);

    if (err != ERR_OK){
        return err;
    }

    // Set the receive function
    udp_recv(elmo_L,udp_receive_callback_L,NULL);
    return err;
}

err_t Go_Baby_Go(char in){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data1[6] = {0};
	uint8_t data2[7] = {0};

	  data1[0] = 'T';
	  data1[1] = 'C';
	  data1[2] = '=';
	  data1[3] = in/10 + 48;
	  data1[4] = in%10 + 48;
	  data1[5] = '\r';

	  data2[0] = 'T';
	  data2[1] = 'C';
	  data2[2] = '=';
	  data2[3] = '-';
	  data2[4] = in/10 + 48;
	  data2[5] = in%10 + 48;
	  data2[6] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,7, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
    	/* copy data to pbuf */
        pbuf_take(p_R, (char*)data2,7);
        pbuf_take(p_L, (char*)data1,6);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);
        //HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
        return ERR_OK;
    }

    return ERR_MEM;
}

err_t Ask_Engines(){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data[6]={0};

	  data[0] = 'M';
	  data[1] = 'O';
	  data[2] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
        /* copy data to pbuf */
        pbuf_take(p_R, (char*)data,6);
        pbuf_take(p_L, (char*)data,6);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);

        return ERR_OK;
    }

    return ERR_MEM;
}
err_t Start_Engines(){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data[5]={0};

	  data[0] = 'M';
	  data[1] = 'O';
	  data[2] = '=';
	  data[3] = '1';
	  data[4] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
		/* copy data to pbuf */
		pbuf_take(p_R, (char*)data,5);
        pbuf_take(p_L, (char*)data,5);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);

        return ERR_OK;
    }

    return ERR_MEM;
}
err_t Stop_Engines(){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data[5]={0};

	  data[0] = 'M';
	  data[1] = 'O';
	  data[2] = '=';
	  data[3] = '0';
	  data[4] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,5, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
		/* copy data to pbuf */
		pbuf_take(p_R, (char*)data,5);
        pbuf_take(p_L, (char*)data,5);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);

        return ERR_OK;
    }

    return ERR_MEM;
}
err_t Ask_For_RPM(){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data[6]={0};

	  data[0] = 'F';
	  data[1] = 'V';
	  data[2] = '[';
	  data[3] = '1';
	  data[4] = ']';
	  data[5] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
		/* copy data to pbuf */
		pbuf_take(p_R, (char*)data,6);
        pbuf_take(p_L, (char*)data,6);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);

        return ERR_OK;
    }

    return ERR_MEM;
}
err_t Ask_For_TEMP(){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data[6]={0};

	  data[0] = 'A';
	  data[1] = 'N';
	  data[2] = '[';
	  data[3] = '1';
	  data[4] = ']';
	  data[5] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
		/* copy data to pbuf */
		pbuf_take(p_R, (char*)data,6);
        pbuf_take(p_L, (char*)data,6);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);

        return ERR_OK;
    }

    return ERR_MEM;
}
err_t Ask_For_VOLT(){

	struct pbuf *p_R;
	struct pbuf *p_L;
	uint8_t data[6]={0};

	  data[0] = 'A';
	  data[1] = 'N';
	  data[1] = '[';
	  data[1] = '6';
	  data[1] = ']';
	  data[2] = '\r';

    /* allocate pbuf from pool*/
	p_R = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);
	p_L = pbuf_alloc(PBUF_TRANSPORT,6, PBUF_POOL);

	if (p_R != NULL && p_L != NULL)
    {
		/* copy data to pbuf */
		pbuf_take(p_R, (char*)data,6);
        pbuf_take(p_L, (char*)data,6);

        /* send udp data */
        udp_send(elmo_R, p_R);
        udp_send(elmo_L, p_L);

        /* free pbuf */
        pbuf_free(p_R);
        pbuf_free(p_L);

        return ERR_OK;
    }

    return ERR_MEM;
}
void MX_LWIP_Process(void)
{
  ethernetif_input(&gnetif);
  sys_check_timeouts();

}

