#include <stdio.h>
#include <stdlib.h>

int main()
{
    int bucket_capacity, leak_rate, num_packets;
    int bucket = 0;

    printf("Enter the bucket capacity: ");
    scanf("%d", &bucket_capacity);

    printf("Enter the leak rate: ");
    scanf("%d", &leak_rate);

    printf("Enter the number of packets: ");
    scanf("%d", &num_packets);

    int packets[num_packets];

    printf("Enter the size of each incoming packet:\n");
    for (int i = 0; i < num_packets; i++)
    {
        scanf("%d", &packets[i]);
    }

    printf("\nLeaky Bucket Simulation:\n");
    printf("Time\tIncoming\tBucket\tLeaked\tRemaining\n");

    for (int i = 0; i < num_packets; i++)
    {
        printf("%d\t%d", i + 1, packets[i]);

        // Add incoming packets to the bucket
        bucket += packets[i];

        if (bucket > bucket_capacity)
        {
            printf("\t%d (Overflow, Dropped %d)", bucket_capacity, bucket - bucket_capacity);
            bucket = bucket_capacity;
        }
        else
        {
            printf("\t%d", bucket);
        }

        // Leak packets at constant rate
        int leaked;
        if (bucket >= leak_rate)
            leaked = leak_rate;
        else
            leaked = bucket;

        bucket -= leaked;

        printf("\t%d\t%d\n", leaked, bucket);
    }

    // Empty remaining packets in bucket
    int time = num_packets + 1;

    while (bucket > 0)
    {
        int leaked;

        if (bucket >= leak_rate)
            leaked = leak_rate;
        else
            leaked = bucket;

        printf("%d\t0\t%d\t%d\t%d\n", time, bucket, leaked, bucket - leaked);

        bucket -= leaked;
        time++;
    }

    return 0;
}
