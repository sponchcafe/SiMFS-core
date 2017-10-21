#! /bin/bash

ps | awk '{print $1}' | xargs kill