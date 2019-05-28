# Find Your Seat (mbed skecth)

This application is based on [mbed-os-example-lorawan](https://os.mbed.com/teams/mbed-os-examples/code/mbed-os-example-lorawan/). It use the [Mbed OS LoRaWanInterface APIs](https://os.mbed.com/docs/mbed-os/v5.12/mbed-os-api-doxy/class_lo_ra_w_a_n_interface.html#afd9143202d548d5ce4292ad05c6d2fa3)

## Add network credentials (ABP Activation method)

For Activation-By-Personalization (ABP) connection method, modify the `mbed_app.json` file to enable ABP. You can do it by simply turning off OTAA:

```json
"lora.over-the-air-activation": false,
```

In addition to that, you need to provide `Application Session Key`, `Network Session Key` and `Device Address`.
You can find it on the device overview page;

```json
"lora.appskey": "{ YOUR_APPLICATION_SESSION_KEY }",
"lora.nwkskey": "{ YOUR_NETWORK_SESSION_KEY }",
"lora.device-address": " YOUR_DEVICE_ADDRESS_IN_HEX  " 
```
The device address need to have that form: 0x12345678

### Duty cycling

In our application we disable the duty cicling and we set a timer, it is not mandatory, if you want to disable follow the step below:

```josn 
"target_overrides": {
	"*": {
		"lora.duty-cycle-on": false
		},
	}
}
```

To modify the timer you can modify the precompiler guidelines 
```
#define TX_TIMER	2500 		// Expressed in ms
```

## Compiling and running the application

If you don't know how to compile and run the application we advise you to refer to the [Official Documentation of Mbed OS 5](https://os.mbed.com/docs/mbed-os/v5.12/introduction/index.html)
