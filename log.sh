#!/usr/bin/env bash

# a little helper to read the logs
# of each entitity

case $1 in
	law)
		echo "Law"
		cat Law/Law.log
		;;
	enf)
		echo "enf"
		cat Enforcer/Enforcer.log
		;;
	judge)
		echo "judge"
		cat Judge/Judge.log
		;;
	company)
		echo "company"
		cat Company/Company.log
		;;
	clean)
		truncate -s 0 Law/Law.log
		truncate -s 0 Enforcer/Enforcer.log
		truncate -s 0 Judge/Judge.log
		truncate -s 0 Company/Company.log
		;;
	*)
esac

