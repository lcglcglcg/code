#/bin/bash

ulimit -c unlimited

local_addr=(
	'124.250.35.164'
	'124.250.35.165'
	'124.250.35.166'
	'124.250.35.167'
	'124.250.35.168'
	'124.250.35.169'
	'124.250.35.170'
	'124.250.35.171'
	'124.250.35.172'
	'124.250.35.173'
	'124.250.35.174'
	'124.251.42.236'
	'124.251.42.237'
	'211.152.7.187'
	'211.152.7.184'
	'211.152.7.185'
	'211.152.7.186'
	'124.251.42.231'
	'124.251.42.234'
	'124.251.42.235'
)

for addr in ${local_addr[@]}
do
./app_baidu_trans $addr >> log/app_baidu_trans-$i-$addr.log
done

#./app_disorderly >> log/app_disorderly-0.log
#./app_disorderly >> log/app_disorderly-1.log
