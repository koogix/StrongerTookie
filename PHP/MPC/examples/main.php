<?php
	/**
	 * 在这个例子中，可以看到 ProcDemo::__construct() 方法只调用了一次
	 * 即子进程是通过 clone() 方式创建了 ProcDemo对象
	 * 而 ProcDemo::__destruct() 方法是进程结束时都会调用的（每个子进程都会调用一次）
	 * 
	 *
	 */
	
	include_once("../classes/MooMPC.class.php");
	
	function procFunc1()
	{
		$moo = MooMPC::getInstance();
		for ($i = 0; $i < 10; $i++)
		{
			printf("%d\t%d: %s\n", $i, $moo->getSid(), __FUNCTION__);
			usleep(rand(1000, 9999));
		}
	}
	
	
	function procFunc2($work, $index)
	{
		$moo = MooMPC::getInstance();
		for ($i = 0; $i < 10; $i++)
		{
			printf("%d\t%d: %s\n", $i, $moo->getSid(), __FUNCTION__);
			usleep(rand(1000, 9999));
		}
	}
	
	class ProcDemo
	{
		
		public function __construct()
		{
			printf("CALL: %s\n", __METHOD__);
		}
		
		public function __destruct()
		{
			$moo = MooMPC::getInstance();
			if ($moo->getSid() == 0)
			{
				printf("CALL: %s\n", __METHOD__);
			}
		}
		
		public function func1($text)
		{
			$moo = MooMPC::getInstance();
			for ($i = 0; $i < 10; $i++)
			{
				printf("%d\t%d: %s(%s)\n", $i, $moo->getSid(), __METHOD__, $text);
				usleep(rand(1000, 9999));
			}
			
		}
		
		public function func2($say, $name)
		{
			$moo = MooMPC::getInstance();
			for ($i = 0; $i < 10; $i++)
			{
				printf("%d\t%d: %s(%s, %s)\n", $i, $moo->getSid(), __METHOD__, $say, $name);
				usleep(rand(1000, 9999));
			}
		}
		public function mainloop()
		{
			printf("CALL: %s\n", __METHOD__);
		}
	}

	//~: run !!!!!!!!
	$pro = new ProcDemo();
	$idx = 1;
	MooMPC::getInstance()
		->init()
		->task('procFunc1')
		->task('procFunc2', 2, 'a', $idx)
		->task(array($pro, 'func1'), 1, 'ok')
		->task(array($pro, 'func2'), 1, 'hello', 'world')
		->exec(function() use($pro) {
			$pro->mainloop();
		});
	
?>