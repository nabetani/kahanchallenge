module Samples; end

# def Samples.many_ones
#   count = 1<<25
#   data = [1]*count
#   [count, data]
# end

def seed
  1
end

ITEM_COUNT = 1e4.to_i

def expected(data)
  src = data.pack("f*").unpack("f*").map(&:to_r)
  sum = 0r
  src.map{ |e|
    sum+=e
    sum.to_f
  }
end

def Samples.flat_rand_0_1
  rng = Random.new(seed)
  Array.new(ITEM_COUNT){ rng.rand(0.0..1.0) }
end

def Samples.flat_rand_m1_1
  rng = Random.new(seed)
  Array.new(ITEM_COUNT){ rng.rand(-1.0..1.0) }
end

def exp_flat_rand(n)
  exprange = (-n..n)
  rng = Random.new(seed)
  Array.new(ITEM_COUNT){ 2**rng.rand(exprange) }
end

def Samples.exp_flat_rand_8
  exp_flat_rand(8)
end

def Samples.exp_flat_rand_32
  exp_flat_rand(32)
end

def Samples.huge_first
  range = (0.9..1.1)
  rng = Random.new(seed)
  [2**22]+Array.new(ITEM_COUNT-1){ |n| rng.rand(range) }
end

def shuffled(n)
  rng = Random.new(seed)
  v = Array.new(n){ rng.rand }
  (0...n).sort_by{ |ix| v[ix] }
end

def Samples.pow8
  rng = Random.new(seed)
  nums = shuffled(ITEM_COUNT)
  nums.map{ |n| (ITEM_COUNT-n)**8 }
end

Samples.singleton_methods.sort.each do |name|
  next unless name==:pow8
  puts name
  data = Samples.send(name)
  values = expected(data)+data
  bins = values.pack("f*")
  File.open( "data", "w" ){ |f| f.write(bins) }
  puts %x(./kahancha data)
end
