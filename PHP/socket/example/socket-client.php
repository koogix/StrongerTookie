<?php

	use koogix\net\Client;
	use koogix\net\Session;
	use koogix\net\StreamBuf;

	require('../src/loader.php');

	KoogixAutoloader::reg();

	Client::factory('127.0.0.1', 50018)
		->on(Session::CONNECTED, function(&$sess)
			{
				printf("Session(%s) %s:%s is connected\n", $sess->id(), $sess->host(), $sess->port());
				$buf = new StreamBuf('ping');
				$sess->send($buf);
			})
		->on(Session::RECIEVED, function(&$strmbuf, &$sess)
			{
				printf("Session(%s) Read: %s\n", $sess->id(), $strmbuf->data());
			})
		->on(Session::DISCONNECTED, function(&$sess)
			{
				printf("Session(%s) Close!\n", $sess->id());
			})
		->run();

?>