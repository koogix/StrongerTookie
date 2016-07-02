<?php

	use koogix\net\Service;
	use koogix\net\Session;

	require('../src/loader.php');

	KoogixAutoloader::reg();

	echo "starting service ...", PHP_EOL;

	Service::factory('0.0.0.0', 50018)
		->on(Session::CONNECTED, function(&$sess, &$connections)
			{
				printf("Session(%s) %s:%s is connected\n", $sess->id(), $sess->host(), $sess->port());
			})
		->on(Session::RECIEVED, function(&$strmbuf, &$sess, &$connections)
			{
				printf("Session(%s) Read: %s\n", $sess->id(), $strmbuf->data());
				foreach ($connections as $session)
				{
					$session->send($strmbuf);
				}
			})
		->on(Session::DISCONNECTED, function(&$sess, &$connections)
			{
				printf("Session(%s) Close!\n", $sess->id());
			})
		->run();

?>