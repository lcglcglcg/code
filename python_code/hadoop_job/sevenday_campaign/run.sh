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

for ((i=1;i<=7;i++))
do
	JOB_DATE_TO_INPUT_PATH=`date -d "$JOB_DATE $i days ago" +%Y-%m/%Y-%m-%d`
	INPUT_PATH="/REPORT_STANDARD_$JOB_DATE_TO_INPUT_PATH/part-*"
	$HADOOP_BIN/hadoop fs -test -e $INPUT_PATH
	if [ $? == 0 ]
	then
	INPUT+=" -input $INPUT_PATH"
	fi
done

OUPUT_PATH="/JOB_TEMP"
OUPUT="-output $OUPUT_PATH"

HADOOP_ENV_JOB_DATE=`date -d "$JOB_DATE 1 days ago" +%Y-%m-%d`
HADOOP_ENV="-cmdenv MONGODB_HOST=$MONGODB_HOST"

cd $JOB_CURT_PATH
$HADOOP_BIN/hadoop fs -rm -r -f $OUPUT_PATH
echo $HADOOP_RUN $HADOOP_JOB_FLAG $MAPPER $REDUCER $INPUT $OUPUT $HADOOP_ENV
$HADOOP_RUN $HADOOP_JOB_FLAG $MAPPER $REDUCER $INPUT $OUPUT $HADOOP_ENV


