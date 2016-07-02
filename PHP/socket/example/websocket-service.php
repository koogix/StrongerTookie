<?php

	use koogix\net\Service;
	use koogix\net\ws\Session;

	require('../src/loader.php');

	KoogixAutoloader::reg();


	class WSApplication
	{

		public function __construct()
		{

		}

		public function getOnConn()
		{
			$closuer = function(&$sess, &$connections)
			{
				$this->onConn($sess, $connections);
			};
			return $closuer;
		}

		public function getOnRecv()
		{
			$closuer = function(&$strmbuf, &$sess, &$connections)
			{
				$this->onRecv($strmbuf, $sess, $connections);
			};
			return $closuer;
		}

		public function getOnClose()
		{
			$closuer = function(&$sess, &$connections)
			{
				$this->onClose($sess, $connections);
			};
			return $closuer;
		}

		private function onConn(&$sess, &$connections)
		{
			printf("Session(%s) %s:%s is connected\n", $sess->id(), $sess->host(), $sess->port());
		}

		private function onRecv(&$strmbuf, &$sess, &$connections)
		{
			printf("Session(%s) Read: %s\n", $sess->id(), $strmbuf->data());
			foreach ($connections as $session)
			{
				$session->send($strmbuf);
			}
		}

		private function onClose(&$sess, &$connections)
		{
			printf("Session(%s) Close!\n", $sess->id());
		}
	}


	echo "starting service ...", PHP_EOL;

	$application = new WSApplication();

	Service::factory('0.0.0.0', 5008)
		// ->setProvider('koogix\net\ws\Session')
		->on(Session::CONNECTED,    Closure::bind($application->getOnConn(),  $application))
		->on(Session::RECIEVED,     Closure::bind($application->getOnRecv(),  $application))
		->on(Session::DISCONNECTED, Closure::bind($application->getOnClose(), $application))
		->ws()->run();

?>