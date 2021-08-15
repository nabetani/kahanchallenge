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

def exp_flat_rand_pm(n)
  exprange = (-n..n)
  rng = Random.new(seed)
  Array.new(ITEM_COUNT){ (rng.rand(2)-0.5)*2*2**rng.rand(exprange) }
end

def Samples.exp_flat_rand_32
  exp_flat_rand(32)
end

def Samples.exp_flat_rand_32_pm
  exp_flat_rand_pm(32)
end

def Samples.huge_first
  range = (0.9..1.1)
  rng = Random.new(seed)
  [2**22]+Array.new(ITEM_COUNT-1){ |n| rng.rand(range) }
end

def Samples.neghuge_first
  range = (0.9..1.1)
  rng = Random.new(seed)
  [-2**22]+Array.new(ITEM_COUNT-1){ |n| rng.rand(range) }
end

def shuffled(n)
  rng = Random.new(seed)
  v = Array.new(n){ rng.rand }
  (0...n).sort_by{ |ix| v[ix] }
end

def sample_pow(n)
  rng = Random.new(seed)
  nums = shuffled(ITEM_COUNT)
  nums.map{ |e| (e+1)**n }
end

def Samples.pow_2
  sample_pow(2)
end

def Samples.pow_8
  sample_pow(8)
end

def normal(a,s)
  rng = Random.new(seed)
  range = (0.0.next_float...1.0.prev_float)
  Array.new(ITEM_COUNT){
    x = rng.rand(range)
    y = rng.rand(range)
    Math.sqrt(-2*Math.log(x))*Math.cos(2*Math::PI*y)*s+a
  }
end

def Samples.normal_0_1
  normal(0,1)
end

def Samples.normal_50_10
  normal(50,100)
end

Samples.singleton_methods.sort.each do |name|
  # next unless /pow/===name.to_s
  puts name
  data = Samples.send(name)
  values = expected(data)+data
  bins = values.pack("f*")
  File.open( "data", "w" ){ |f| f.write(bins) }
  s=%x(./kahancha data)
  File.open( "py/#{name}.csv", "w" ) do |f|
    f.puts s
  end
end
