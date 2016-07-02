<?php
	/**
	 * 异常类
	 * 
	 * 用于方便抛出或显示socket相关的错误信息
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */
	
	namespace koogix\net;

	/**
	 * 异常类
	 * 
	 * newThrow() 方法抛出异常
	 * display() 方法只显示错误
	 *
	 */
	class Exception extends \Exception
	{
		/**
		 * 抛出异常
		 *
		 * @param  Resource $socket 网络连接句柄
		 */
		public static function newThrow($socket)
		{
			$errno   = socket_last_error($socket);
			$message = socket_strerror($errno);
			throw new Exception(sprintf("Socket Error(%d): %s", $errno, $message));
		}

		/**
		 * 显示错误信息
		 *
		 * @param  Resource $socket 网络连接句柄
		 */
		public function display($socket)
		{
			$errno   = socket_last_error($socket);
			$message = socket_strerror($errno);
			printf("Socket Warning(%d): %s%s", $errno, $message, PHP_EOL);
		}
	}

?>