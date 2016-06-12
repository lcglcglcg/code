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

HADOOP_HOME="/usr/local/hadoop/"
HADOOP_BIN="$HADOOP_HOME/bin"
HADOOP_STREAMING="/usr/local/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.3.0-cdh5.1.2.jar"
HADOOP_RUN="$HADOOP_BIN/hadoop jar $HADOOP_STREAMING"
#HADOOP_JOB_FLAG="-D mapred.reduce.tasks=1"
#HADOOP_JOB_FLAG="-D mapred.reduce.tasks=30"
#JOB_HOME_PATH="/ad_system/hadoop_job/temp"

JOB_HOME_PATH="/home/hadoop/hadoop_job"
MONGODB_HOST="192.168.119.49"
MONGODB_PORT="27017"

if [ $1 ]
then
JOB_DATE=$1
else 
JOB_DATE=`date +%Y-%m-%d`
fi
JOB_LIST=(
#        'report_media_58'
#        'report_standard_58'
        'report_media_haomeit_qiuyi'
        'report_standard_haomeit_qiuyi'
)

for i in ${JOB_LIST[@]}
do
        echo -e "\033[31;49;1m$i\033[32;49;1m Start ...\033[32;49;0m"
        JOB_CURT_PATH=$JOB_HOME_PATH/$i
        sh ${JOB_HOME_PATH}/$i/run.sh
done



