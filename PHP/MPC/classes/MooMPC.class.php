<?php
	/**
	 * 基于PHP的PCNTL与POSIX扩展的多任务类
	 * 用于分离任务同时执行多个任务
	 * 各个任务间没有什么强的依赖关系
	 *
	 * 比如一个百万的消息队列，
	 * 直接一个个去跑需要累加的一百万个周期
	 * 但分割成50个子任务，每个任务只需要2万个周期
	 * 而这些任务是同时进行的，即最优的情况下比单个发送快了50倍！
	 *
	 * @category  PHP
	 * @package   Stroger.Tookie.PHP
	 *
	 * @author    李文强 <lwq53@126.com>
	 * @version   0.0.1
	 * @copyright 2015 - 2025
	 */

	/**
	 * 最大子进程数
	 *
	 * 为了性能考虑，不能超过设置的最大子进程数
	 */
	define('MAX_PROCESS', 50);

	/**
	 * 异常
	 *
	 */
	class MooException extends Exception {}
	
	/**
	 * Multiple Processes Control
	 *
	 * 使用 POSIX 与 PCNTL 模块实现多进程程序
	 *
	 */
	class MooMPC
	{
		/**
		 * @var MooMPC
		 */
		private static $mpInstance = NULL;
		
		/**
		 * @var int
		 */
		private $procId = 0;
		
		/**
		 * @var int
		 */
		private $sonId  = 0;
		
		/**
		 * @var int
		 */
		private $count  = 0;
		
		/**
		 * @var Array
		 */
		private $arrid	= array();
		
		/**
		 * @var Array
		 */
		private $called = array();
		
		/**
		 * 获取MooMPC单例对象
		 *
		 * @access public
		 *
		 * @return MooMPC  单例对象
		 */
		public static function getInstance()
		{
			if (self::$mpInstance == NULL)
			{
				self::$mpInstance = new MooMPC();
			}
			return self::$mpInstance;
		}
		
		/**
		 * 初始化检测
		 *
		 * init()函数主要用于检测是否启用了pcntl及posix模块
		 *
		 * @access public
		 *
		 * @return MooMPC
		 */
		public function init()
		{
			$checkmod = array('pcntl', 'posix');
			foreach ($checkmod as $phpmod)
			{
				if (!extension_loaded('pcntl'))
				{
					throw new MooException(sprintf("PHP MODULE [%s] NOT LOADED", $phpmod));
				}	
			}
			return $this;
		}
		
		/**
		 * 构造函数
		 *
		 * @access private
		 *
		 */
		private function __construct()
		{
			$this->procId = getmypid();
		}
		
		/**
		 * 释构函数
		 *
		 * @access public
		 *
		 */
		public function __destruct()
		{
			if (!empty($this->arrid))
			{
				foreach ($this->arrid as $pid)
				{
					pcntl_waitpid($pid, $status);
				}
			}
		}
		
		/**
		 * 取父进程PID
		 *
		 * @return int 
		 */
		public function getPid()
		{
			return $this->procId;
		}
		
		/**
		 * 取子进程ID
		 *
		 * @return int
		 */
		public function getSid()
		{
			return $this->sonId;
		}
		
		/**
		 * 设置子进程函数
		 *
		 * add(callback, threads, ...)
		 *
		 * 第一个参数为子进程要执行的函数
		 * * 可以是 functionname 或 array(instance, method)
		 * 第二个参数为要启动多少个子进程
		 * 第三个及以后的参数为子进程函数的参数
		 *
		 * @usage $moo->add('functionname', 1, 'parameter list')
		 * @usage $moo->add(array('classInstance', 'methodname'), 1, 'parameter list')
		 *
		 * @param  CALLBACK  子进程函数，可以是函数名，或者类实例关联的方法名
		 * @param  int       启动的进程数
		 * @param  ...       对应 CALLBACK 的参数列表
		 * @return MooMPC
		 */
		public function task()
		{
			$num = func_num_args();
			if (empty($num))
			{
				return true;
			}
			$option = array();
			$args = func_get_args();
			$call = array_shift($args);
			// ~ 检测方法是否存在
			if ((is_array($call) && method_exists($call[0], $call[1])) || (is_string($call) && function_exists($call)))
			{
				$option[] = $call;
			}
			else
			{
				throw new MooException("PROCESS FUNCTION NOT EXISTS");
			}
			// ~ 未设置任务数则自动为 1
			if (empty($args))
			{
				$option[] = 1;
				$option[] = array();
			}
			else
			{
				$count = array_shift($args);
				$count = intval($count);
				if ($count < 1)
				{
					$count = 1;
				}
				$option[] = $count;
				$option[] = $args;
			}
			$this->count += $option[1];
			if ($this->count > MAX_PROCESS)
			{
				throw new MooException("TOO MORE PROCESS");
			}
			$this->called[] = $option;
			return $this;
		}
		/**
		 * 启动多任务
		 *
		 * 根据task设置的任务启动子进程
		 *
		 * @param  callabled 函数或方法
		 *         可以使用匿名函数 >> function() use() {}
		 *         父进程创建完成子进程后执行
		 *
		 * @return void
		 */
		public function exec($fun = NULL)
		{
			if (empty($this->called))
			{
				return true;
			}
			$flag = true;
			foreach ($this->called as $option)
			{
				for ($i = 0; $i < $option[1]; $i ++)
				{
					$flag = $this->proc($option[0], $option[2]);
					if (!$flag)
					{
						break;
					}
				}
				if (!$flag)
				{
					break;
				}
			}
			if ($flag && !empty($fun) && is_callable($fun))
			{
				$fun();
			}
		}
		
		/**
		 * 执行子进程任务
		 *
		 * 使用 pcntl_fork() 创建并执行子进程
		 * 父进程返回true，子进程返回false
		 *
		 * @param  CALLBACK  子进程函数
		 * @param  Array     参数列表数据
		 * @return bool 
		 */
		private function proc($call, $param)
		{
			$pid = pcntl_fork();
			if ($pid == -1)
			{
				throw new MooException("FORK PROCESS ERROR");
			}
			else if ($pid > 0)
			{
				$this->arrid[] = $pid;
				return true;
			}
			else
			{
				$this->arrid = NULL;
				$this->sonId = posix_setsid();
				call_user_func_array($call, $param);
			}
			return false;
		}
	}//~: ___END_CLASS_MOOMPC_____
	
?>