module Samples
end

def Samples.ones
  count = 1<<25
  data = [1]*count
  [ count, data]
end

Samples.singleton_methods.each do |name|
  expected, data = Samples.send(name)
  values = [expected] + data
  bins = values.pack("f*")
  File.open( "data", "w" ){ |f| f.write(bins) }
  puts %x(./a.out data)
end
