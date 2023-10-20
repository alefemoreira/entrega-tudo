instances=( "n9k5_A" "n9k5_B" "n9k5_C" "n9k5_D" "n14k5_A" "n14k5_B" "n14k5_C" "n14k5_D" "n22k3_A" "n22k3_B" "n22k3_C" "n22k3_D" "n31k5_A" "n31k5_B" "n31k5_C" "n31k5_D" "n43k6_A" "n43k6_B" "n43k6_C" "n43k6_D" "n64k9_A" "n64k9_B" "n64k9_C" "n64k9_D" "n120k7_A" "n120k7_B" "n120k7_C" "n120k7_D" "n199k17_A" "n199k17_B" "n199k17_C" "n199k17_D" )
seeds=( 2 3 5 7 11 13 17 19 23 29 )

for i in "${instances[@]}"; do
  for op in {1..3}; do
    for s in "${seeds[@]}"; do
      continue
    done
  done
done

