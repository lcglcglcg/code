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

cd $JOB_CURT_PATH
$JOB_CURT_PATH/bin/mongo $MONGODB_HOST/adx $JOB_CURT_PATH/mongo_job.js

