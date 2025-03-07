"""Code modified from the example program to show how to read a multi-channel time series from LSL at https://github.com/OpenBCI/OpenBCI_GUI/blob/master/Networking-Test-Kit/LSL/lslStreamTest.py."""
import asyncio
from pylsl import StreamInlet, resolve_streams
import time
from bleak import BleakClient


DEVICE_UUID = "46254AD5-6E40-42C5-6FE6-17AFBCEEF543"#edit this, to do so download a BLE scanner app and find the characteristics there.
CHARACTERISTIC_UUID = "2A56"  # this should saty the same 

client = BleakClient(DEVICE_UUID)

#thresholds
time_thres = 500
f_thres = 1
lr_thres = .75

async def send_ble_command(command):
    if not client.is_connected:
        await client.connect()
    await client.write_gatt_char(CHARACTERISTIC_UUID, command.encode())

async def main():
    prev_time = 0  

    # Resolve LSL streams to get EMG data
    print("Resolving LSL streams...")
    streams = resolve_streams()
    inlet = StreamInlet(streams[0])

    while True:
        # Get EMG data sample and its timestamp
        samples, timestamp = inlet.pull_sample()

        # Get current time in milliseconds
        curr_time = int(round(time.time() * 1000))

        # Check for EMG spikes and send commands via BLE
        if (samples[0] >= f_thres) and (curr_time - time_thres > prev_time) and (samples[1] >= f_thres) and (curr_time - time_thres > prev_time):  #Both eyebrows spike
            prev_time = curr_time  
            await send_ble_command('f') 

        elif (samples[0] >= lr_thres) and (curr_time - time_thres > prev_time) and not (samples[1] >= lr_thres) and (curr_time - time_thres > prev_time):  #left eyebrow spikes and right does'nt
            prev_time = curr_time  
            await send_ble_command('l') 

        elif (samples[1] >= lr_thres) and (curr_time - time_thres > prev_time) and not (samples[0] >= lr_thres) and (curr_time - time_thres > prev_time) :  #rigth eyebrow spikes and left does'nt
            prev_time = curr_time  
            await send_ble_command('r')  

        elif curr_time - time_thres > prev_time:  
            prev_time = curr_time  
            await send_ble_command('s')  #no spike

        await asyncio.sleep(0.01)  

# Start the main code
asyncio.run(main())
