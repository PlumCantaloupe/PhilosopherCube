#!/bin/sh

#testing
consumer_key=""
consumer_secret=""
access_token=""
access_secret=""

url="http://api.twitter.com/1/statuses/mentions.xml"

http_get -h Authorization "$(oauth_sign $consumer_key $consumer_secret $access_token $access_secret GET "$url" screen_name="$1" since_id="$2" )" "$url?screen_name=$1&since_id=$2"
