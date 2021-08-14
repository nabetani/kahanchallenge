module Samples; end

# def Samples.many_ones
#   count = 1<<25
#   data = [1]*count
#   [count, data]
# end

def seed
  1
end

def expected(data)
  data.pack("f*").unpack("f*").map(&:to_r).sum.to_f
end

def Samples.flat_rand_0_to_1
  rng = Random.new(seed)
  data = Array.new(1e5){ rng.rand }
  [expected(data), data]
end

def Samples.rand_exp_1e5
  rng = Random.new(seed)
  data = Array.new(1e5){ 2**(rng.rand*20) }
  [expected(data), data]
end

def Samples.flat_exp_1e5pm
  rng = Random.new(seed)
  data = Array.new(1e5){ (rng.rand(2)==0 ? -1 : 1)*2**(rng.rand*20) }
  [expected(data), data]
end

Samples.singleton_methods.each do |name|
  expected, data = Samples.send(name)
  values = [expected] + data
  bins = values.pack("f*")
  File.open( "data", "w" ){ |f| f.write(bins) }
  puts name
  puts %x(./kahancha data)
end
