<?php
	/**
	 * 自动加载 koogix 命名空间下的类库
	 * 
	 * 可以包含本文件使用
	 * KoogixAutoloader::reg()
	 * 
	 * 也可以使用自行实现autoload
	 * 或直接包含对应koogix目录下的类库文件
	 *
	 * @author  李文强 (Wenqiang Lee) <lwq53@126.com>
	 * @version 0.0.1
	 */

	// ~ 目录分隔符
	define('_', DIRECTORY_SEPARATOR);

	// ~ 项目路径
	define('KOOGIX_LOADER', __DIR__._);

	// ~ php类库后缀
	define('CLASS_EXTNAME', '.php');

	/**
	 * Autoloader实现类
	 *
	 * 只自动查找koogix命名空间下的类库
	 *
	 */
	class KoogixAutoloader
	{
		/**
		 * 注册autoload函数
		 *
		 * @static
		 */
		public static function reg()
		{
			spl_autoload_register(__CLASS__.'::autoload');
		}

		/**
		 * autoload方法实现
		 *
		 * @static
		 * @param string $classname
		 */
		public static function autoload($classname)
		{
			$arr = explode('\\', $classname);
			if (count($arr) > 1 && $arr[0] == 'koogix')
			{
				$filepath = KOOGIX_LOADER . implode(_, $arr) . CLASS_EXTNAME;
				if (is_file($filepath))
				{
					include($filepath);
				}
			}
			return true;
		}
	}
?>