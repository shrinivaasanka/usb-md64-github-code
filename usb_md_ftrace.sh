#!/bin/bash
tracefs=/sys/kernel/debug
echo nop > $tracefs/tracing/current_tracer
echo 0 > $tracefs/tracing/tracing_on
echo $$ > $tracefs/tracing/set_ftrace_pid
#echo 3857 > $tracefs/tracing/set_ftrace_pid
echo function > $tracefs/tracing/current_tracer
echo 1 > $tracefs/tracing/tracing_on
exec "$@"
cat /sys/kernel/debug/tracing/trace 2>&1 > ftrace.log
