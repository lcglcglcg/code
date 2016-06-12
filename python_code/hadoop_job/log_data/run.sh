#/bin/bash

export HADOOP_HOME;
export HADOOP_BIN;
export HADOOP_STREAMING;
export HADOOP_RUN;
export HADOOP_JOB_FLAG;
export MONGODB_HOST;
export MONGODB_PORT;
export JOB_HOME_PATH;
export JOB_CURT_PATH;
export JOB_DATE;

MAPPER=" -mapper mapper.py -file mapper.py"
REDUCER=" -reducer reducer.py -file reducer.py"
#REDUCER=" -reducer cat"

JOB_DATE_TO_INPUT_PATH=`date -d "$JOB_DATE 1 days ago" +%Y-%m/*%Y-%m-%d`
JOB_DATE_TO_OUPUT_PATH=`date -d "$JOB_DATE 1 days ago" +%Y-%m/%Y-%m-%d`

INPUT_PATH="/LOG_SHOW_$JOB_DATE_TO_INPUT_PATH"
$HADOOP_BIN/hadoop fs -test -e $INPUT_PATH
if [ $? == 0 ]
then
INPUT="-input $INPUT_PATH"
else
	echo -e "\033[31;49;1mINPUT FILE ERROR!\033[32;49;0m"
	exit 1
fi

INPUT_PATH="/LOG_CLICK_$JOB_DATE_TO_INPUT_PATH"
$HADOOP_BIN/hadoop fs -test -e $INPUT_PATH
if [ $? == 0 ]
then
INPUT+=" -input $INPUT_PATH"
fi

THIS_DATE=`date +%Y-%m-%d`
if [ $THIS_DATE == $JOB_DATE ]
then
	INPUT_PATH="/LOG_TODAY_TEMP/CLICK"
	$HADOOP_BIN/hadoop fs -test -e $INPUT_PATH
	if [ $? == 0 ]
	then
		INPUT+=" -input $INPUT_PATH"
	fi
else
	JOB_DATE_TO_INPUT_PATH=`date -d "$JOB_DATE" +%Y-%m/*%Y-%m-%d`
	INPUT_PATH="/LOG_CLICK_$JOB_DATE_TO_INPUT_PATH"
	$HADOOP_BIN/hadoop fs -test -e $INPUT_PATH
	if [ $? == 0 ]
	then
		INPUT+=" -input $INPUT_PATH"
	fi
fi

OUPUT_PATH="/LOG_DATA_$JOB_DATE_TO_OUPUT_PATH"
OUPUT=" -output $OUPUT_PATH"

if [ "$INPUT" ]
then
	cd $JOB_CURT_PATH
	$HADOOP_BIN/hadoop fs -rm -r -f $OUPUT_PATH
	echo $HADOOP_RUN $HADOOP_JOB_FLAG $MAPPER $REDUCER $INPUT $OUPUT
	$HADOOP_RUN $HADOOP_JOB_FLAG $MAPPER $REDUCER $INPUT $OUPUT
else
	echo -e "\033[31;49;1mINPUT FILE ERROR!\033[32;49;0m"
fi


