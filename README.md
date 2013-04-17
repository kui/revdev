Revdev
=================

revdev is a ruby binding to handling event devices.

Installation
--------------

Add this line to your application's Gemfile:

```rb
gem 'revdev'
```

And then execute:

```sh
$ bundle
```

Or install it yourself as:

```sh
$ gem install revdev
```


Example Usage
----------------------

In terminal:

```sh
ruby -Ilib sample/device_name /dev/input/event*
ruby -Ilib sample/key_dump /dev/input/event4
...
```

Contributing
-------------------------

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Added some feature'`)
	* Please check tests to execute `$ rake`
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
