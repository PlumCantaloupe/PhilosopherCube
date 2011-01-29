#!/bin/sh

#testing
consumer_key=""
consumer_secret=""
access_token=""
access_secret=""

url="http://api.twitter.com/1/statuses/update.xml"

http_post -h Authorization "$(oauth_sign $consumer_key $consumer_secret $access_token $access_secret POST "$url" status="$*")" "$url" status="$*"
