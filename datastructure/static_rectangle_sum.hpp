#ifndef STATIC_RECTSUM_HPP
#define STATIC_RECTSUM_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename T>
struct static_rectangle_sum{
	int n;
    const T TINF = std::numeric_limits<T>::max() / 2;
    vector<int> x;
	vector<vector<pair<int, T>>> dat;
    vector<vector<T>> sum

	static_rectangle_sum(vector<tuple<int, int, T>> &vec){
        // sort vec by x
        sort(vec.begin(), vec.end());
        for(int i=0; i<(int)vec.size(); i++) x.push_back(get<0>(vec[i]));

        //let n be power to 2
		n=1;
		while(n<(int)vec.size()) n<<=1;

        // resiz dat and sum
		dat.assign(2*n-1, {});
        sum.assign(2*n-1, {});

        // initialize dat and sum
		for(int i=0; i<(int)vec.size(); i++){
            dat[i+n-1].push_back({get<1>(vec[i]), get<2>(vec[i])});
            sum[i+n-1].push_back(0);
            sum[i+n-1].push_back(get<2>(vec[i]));
        }

		for(int i=n-2; i>=0; i--){
			dat[i].resize((int)dat[i*2+1].size()+(int)dat[i*2+2].size());
			merge(dat[i*2+1].begin(),dat[i*2+1].end(),
				dat[i*2+2].begin(),dat[i*2+2].end(),
				dat[i].begin()
			);

            sum[i].resize(dat[i].size()+1, 0);
            for(int j=1; j<=(int)dat[i].size(); j++){
                sum[i][j]=sum[i][j-1]+dat[i][j-1].second;
            }
		}
	}

    // response sum(*<y) in [a, b)
    T query(int a, int b, int y){
        a = lower_bound(x.begin(), x.end(), a)-x.begin();
        b = lower_bound(x.begin(), x.end(), b)-x.begin();
        return query_sub(a, b, y);
    }

    // response sum(ly <= * < uy) in [a, b)
    T query(int a, int b, int ly, int uy){
        return query(a, b, uy)-query(a, b, ly);
    }

    T query_sub(int a, int b, int y, int k=0, int l=0, int r=-1){
        if(r<0) r=n;
		if(b<=l||r<=a){
            return 0;
        }else if(a<=l&&r<=b){
            return sum[k][lower_bound(dat[k].begin(),dat[k].end(), make_pair(y, -TINF))-dat[k].begin()];
        }else{
            return query_sub(a, b, y, k*2+1, l, (l+r)/2)+query_sub(a, b, y, k*2+2, (l+r)/2, r);
        }
    }
};
#endif