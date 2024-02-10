bundle binstubs --all
cd ../src/
make
cd -
cp ../src/db ./
bundle exec rspec spec/db_test.rb
